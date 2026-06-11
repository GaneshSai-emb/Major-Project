#include <LPC214x.H>  /* LPC21xx definitions*/
#include"uart_interuppt.h"
#include <string.h>
#include<time.h>
#include"lcd.h"
#include "rtc.h"
#define UART_INT_ENABLE 1 //enable uart0 interrupt functionality
 
char buff[200],dummy; // buffer to store the data and dummy variable
unsigned char i=0,ch,r_flag;
/* uart0 interrupt service routine this function is automatically called whenever interrupt occured*/

void UART0_isr(void) __irq  

{

  if((U0IIR & 0x04)) //it checks whether the interrupt is received or not
 
  {

		ch = U0RBR;
		if(ch!=0x0d)
		{	                  /* Read to Clear Receive Interrupt */
			buff[i++] = ch; 
		}
		else if(i>7)
		{
		buff[8] = '\0';// terminates after 8 characters 
			i=0;// reset buffer index
		r_flag= 1;// set flag indicating data reception is completed 
	
		}

  }

  else

  {

      dummy=U0IIR; //Read to Clear transmit interrupt

  

  }

   VICVectAddr = 0; /* dummy write */

}


void InitUART0 (void) /* Initialize Serial Interface       */ 

{  

              
  PINSEL0 = 0x00000005; /* Enable RxD0 and TxD0              */

  U0LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit     */

  U0DLL = 97;           /* 9600 Baud Rate @ CCLK/4 VPB Clock  */

  U0LCR = 0x03;         /* DLAB = 0  */

  

  #if UART_INT_ENABLE > 0


  VICIntSelect = 0x00000000; // IRQ

  VICVectAddr0 = (unsigned)UART0_isr;

  VICVectCntl0 = 0x20 | 6; /* UART0 Interrupt */

  VICIntEnable = 1 << 6; 

  

  

  /* Enable UART0 Interrupt */

 

 // U0IIR = 0xc0;

 // U0FCR = 0xc7;

  U0IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */   

             

  #endif

						

}


void UART0_Tx(char ch)  /* Write character to Serial Port    */  

{ 

  while (!(U0LSR & 0x20));

  U0THR = ch;                

}


char UART0_Rx(void)    /* Read character from Serial Port   */

{                     

  while (!(U0LSR & 0x01));

  return (U0RBR);

}





void UART0_Str(char *s)

{

   while(*s)

       UART0_Tx(*s++); //sends characters one by one

}

//sends a unsigned integer through uart0
void UART0_Int(unsigned int n)

{

  unsigned char a[10]={0,0,0,0,0,0,0,0,0,0};

  int i=0;

  if(n==0)

  {

    UART0_Tx('0');

	return;

  }

  else

  {

     while(n>0)

	 {

	   a[i++]=(n%10)+48;//convert digit to ascii

	   n=n/10;

	 }

	 --i;

	 for(;i>=0;i--)

	 {

	   UART0_Tx(a[i]);

	 }

   }

}


void UART0_Float(float f)

{

  int x;

  float temp;

  x=f;

  UART0_Int(x);

  UART0_Tx('.');

  temp=(f-x)*100;

  x=temp;

  UART0_Int(x);

}

//some software delay
void  DelayS(unsigned int  dly)

{  unsigned int  i;


   for(; dly>0; dly--) 

      for(i=1200000; i>0; i--);

}

