//the main file of this project
/*all header defintions
Main application file for rfid and otp based authenticatiopn system
featues are:
1.rfid tag authentication
2. otp generation and validation
3.gsm based otp transmission
4.rtc based timeout monitoring
5.lcd user interface
6.keypad based otp entry
*/
#include<stdio.h>    
#include<lpc214x.h>
#include<string.h>
#include"lcd.h"
#include "rtc.h"
#include "uart_interuppt.h"
#include "gsm.h"
#include "kpm.h"
#include "delay.h"
char vaildtag[]="12603355";
s32 hour,min,sec;
s32 date,month,year,day;
char otp[5];
char enter_otp[5];	
s32 otp_time;
s32  now_time;
int attempts;
/*declaration of the functions used */
void generate_otp(void);
int check_otp(void);
void send_otp(void);
int validate_otp(void);							 

int main()
{
 //intilizations of all the interfaces
 IODIR0|=1<<12;// configure po.12 as output direction for led
 InitUART0();// intialize uart0
 InitUART1();//intialize uart1
 InitLCD(); //intialize lcd
 RTC_Init();  
 gsm_init();
 InitKPM();
 enable_ent1();
   while(1)
   {
		
	r_flag=0;
	do
	{
	GetRTCTimeInfo(&hour,&min,&sec);
        GetRTCDateInfo(&date,&month,&year);
        GetRTCDay(&day);
	DisplayRTCTime(hour,min,sec);
        DisplayRTCDate(date,month,year);
        DisplayRTCDay(day);
	}while(r_flag1!=2); 

         CmdLCD(0x01);
	 StrLCD("RFID TAG:");
	 CmdLCD(0xc0);
	 StrLCD(buff1);
	 delay_ms(2000);
	 CmdLCD(0x01);
	 r_flag1=0;
	 CmdLCD(0x01);
	 //checks whether the given tag is valid or not
	 if (strcmp(vaildtag,buff1)==0)
	 {
	 CmdLCD(0x01);
	 StrLCD("valid Tag");
	 delay_ms(1000);
         GetRTCTimeInfo(&hour,&min,&sec);
	 otp_time= (hour*3600)+(min*60)+sec;
         generate_otp(); //generate otp 
	 send_otp(); // sends otp
        attempts=0;
	while(attempts<3)
	{
	 if(validate_otp())//checks whether the otp entered is correct or not
	 {
	 if(check_otp())//checks the time expiry
	 {
	 CmdLCD(0x01);
	 StrLCD("Acess Granted"); //displays the access granted
	 delay_ms(500);
	 CmdLCD(0x01);
	 StrLCD("Door open");
	 IOSET0=1<<12;  // led will be enabled here
	 delay_ms(500);
	 CmdLCD(0x01);
         StrLCD("Door close"); 
	 IOCLR0=1<<12;  // after a while disable the led
	 delay_ms(2000);
	 break;
	 }
	 else // this part executes when timeout is happened
	 {
		 CmdLCD(0x01);
	 	 StrLCD("OTP EXPIRED");
		 delay_ms(1000);
		 break;
	 }
	 }
	 else 
	 {
       attempts++;   //increase the attempts if the password is wrong up to 3 attempts
	   CmdLCD(0x01);
	   StrLCD("WRONG OTP");
	   delay_ms(500);
	   if(attempts >=3)			   
       {
	   	CmdLCD(0x01);
		StrLCD("acesses denied");
		delay_ms(2000); 
	   }
	  }
	   }
	 }
	 else 
	 {
	 CmdLCD(0x01);
	 StrLCD("Invalid card");
	 delay_ms(500);
	 }
	 CmdLCD(0x01);
}	                      
}

void generate_otp(void)  // this function will help you to generate random otp 
{
otp[0]= ((hour%10)+'0');
otp[1]= ((min%10)+'0');
otp[2]= ((sec%10)+'0');
otp[3]= (((hour+min*sec)/10)+'0');
otp[4]=  '\0';
}

void send_otp(void)
{
if(gsm_flag==1)  //checks if gsm intialization done or failed
{
UART0_Str("AT+CMGS=\"8096261977\"\r\n");
delay_ms(1000);
UART0_Str("YOUR OTP IS:");
UART0_Str(otp);  //transmitts the otp to gsm module to send sms to mobile
UART0_Str("Valid for 3 mins");
UART0_Tx(0x1A);
delay_ms(1000);
}						   
}

//this function will helps to check the entered otp is correct or not  returns 1 if entered otp is valid  0 on not valid
int validate_otp(void)
{
int i,ch;
u8 key;
CmdLCD(0x01);

CmdLCD(0x80);
StrLCD("ENTER OTP");
CmdLCD(0xc0);
i=0;
while(1)
{
if(ColScan()==1)
{
key=KeyScan();
if(key==10)
{
if(i>=0)
{
i--;
enter_otp[i]='\0';
CmdLCD(0xc0+i);
CharLCD(' ');
CmdLCD(0xc0+i);
}
}
else if(key==16)
{
if(i==4)
break;
}
else 
{
if(i<4)
{
 enter_otp[i]=key+48;
 CharLCD(key+48);
 i++;
}
}
}
}
enter_otp[4]='\0';
if(strcmp(otp,enter_otp)==0)
{
return 1;
}
return 0;
}
// checks the time validation whether it is expired or not  returns 1 if not expired and returns 0 on expiry
int check_otp()
{
   GetRTCTimeInfo(&hour,&min,&sec);
	  now_time=(hour*3600)+(min*60)+sec;
	 if((now_time - otp_time) > 60)
	 {							   
	 return 0;
	 }

 return 1;
}
