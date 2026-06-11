#include<stdio.h>
#include<lpc21xx.h>

void eint1_isr(void)__irq  //This Function will contain the isr of interruppt
{
	adminmode();// this function will help to enter into the admin mode
	EXTINT=(1<<1);//clear ENT1 interrupt flag
	VICVectAddr=0;//inform vic that isr execution is complete
}

/*configure and enable external interrupt1*/
void enable_ent1(void)
{
//configure p0.14 as ent1 pin
PINSEL0&=~(3<<28);  
PINSEL0|=(2<<28);
//enable the interrupt in vic
VICIntEnable=1<<15;
//assign isr address to vector slot2
VICVectAddr2 = (unsigned)eint1_isr;
//configure vector slot2 for ent1;
VICVectCntl2 = (1<<5)|15;
EXTMODE|=1<<1;//set it as edge trigger interuppt
//EXTPOLAR=0<<1; 
}

