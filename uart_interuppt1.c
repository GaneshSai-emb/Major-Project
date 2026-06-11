#include <LPC214x.H>  /* LPC21xx definitions         */
#include"uart_interuppt.h"
#include <string.h>
#include<time.h>
#include"lcd.h"
#include "rtc.h"
#define UART_INT_ENABLE 1  //enable the uart1 interrupt support
char buff1[200],dummy1; //buffer to received data and dummy variable
unsigned char i1=0,ch1,r_flag1;//buffer index,reception flag,received character
void InitUART1 (void);
/* uart1 interrupt support 
this function is executed automatically whenever a uart1 interrupt occurs  it receives data from uart1 and stores into buffer*/
void UART1_isr(void) __irq

{

  if((U1IIR & 0x04)) //check if the interrupt is received or not

  {

		ch1= U1RBR;
		if(ch1==0x02)
		{	/* Read to Clear Receive Interrupt */

			//buff1[i1++] = ch1; 
			r_flag1=1;
		}
		else if((ch1!=0x03)&&(r_flag1==1))
		{
		buff1[i1++]=ch1;
		}
		else 
		{
		 buff1[i1]='\0';
		 i1=0;
		 r_flag1=2;
		}

  }

  else

  {

      dummy1=U1IIR; //Read to Clear transmit interrupt

  

  }

   VICVectAddr = 0; /* dummy write */

}


void InitUART1 (void) /* Initialize Serial Interface       */ 

{  

            		

  PINSEL0 |= 0x00050000; /* Enable RxD0 and TxD0              */

  U1LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit     */

  U1DLL = 97;           /* 9600 Baud Rate @ CCLK/4 VPB Clock  */

  U1LCR = 0x03;         /* DLAB = 0  */

  

  #if UART_INT_ENABLE > 0


  VICIntSelect = 0x00000000; // IRQ

  VICVectAddr1 = (unsigned)UART1_isr;

  VICVectCntl1 = 0x20 | 7; /* UART0 Interrupt */

  VICIntEnable |= 1 << 7; 

  

  

  /* Enable UART0 Interrupt */

 

 // U0IIR = 0xc0;

 // U0FCR = 0xc7;

  U1IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */   

             

  #endif

						

}


void UART1_Tx(char ch)  /* Write character to Serial Port    */  

{ 

  while (!(U1LSR & 0x20));

  U1THR = ch;                

}


char UART1_Rx(void)    /* Read character from Serial Port   */

{                     

  while (!(U1LSR & 0x01));

  return (U1RBR);

}





void UART1_Str(char *s)  //sends string

{

   while(*s)

       UART1_Tx(*s++); //sends character one by one

}

// sends a integer value
void UART1_Int(unsigned int n)

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

	   a[i++]=(n%10)+48;

	   n=n/10;

	 }

	 --i;

	 for(;i>=0;i--)

	 {

	   UART1_Tx(a[i]);

	 }

   }

}

//sends a float value
void UART1_Float(float f)

{

  int x;

  float temp;

  x=f;

  UART1_Int(x);

  UART1_Tx('.');

  temp=(f-x)*100;

  x=temp;

  UART1_Int(x);

}


