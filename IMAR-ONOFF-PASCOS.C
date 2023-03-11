/**************************************************** *********
//Project: IMAR-TEST.prj
// Device: MS80Fxx01
// Memory: Flash 1KX14b, EEPROM 256X8b, SRAM 64X8b
// Author: 
//Company: 
//Version:
//   Date: 
*Pin Definition:
*----------------
* VDD------------|1(VDD) (GND)16|------------GND
* SW1------------|2(RA2) (RA4)15|---------ROLE1/OUT1
* NC-------------|3(RA1) (RA5)14|---------ROLE2/OUT2
* SW2------------|4(RA3) (RA0)13|-------------NC
************************************************************/
//===========================================================
//===========================================================
#include   "SYSCFG.h";
#define _XTAL_FREQ       4000000         //4T
#define SW1 RA2
#define SW2 RA3
#define LED1 RA4
#define LED2 RA5

//#define DEL_MS 70
unsigned char DEL_MS = 70, time_out = 0, num = 0;
int  t,i, k, j=1, count = 0;
//char state_out = 0;
int dem =1;

unsigned char Read_8bit_Data_Eeprom(unsigned char EepromAddress)
{
    unsigned char EepromData;
    CLRWDT();
    EEADR = EepromAddress;
    RD = 1;
    EepromData = EEDAT;
    RD = 0;
    return(EepromData);
}

void Write_8bit_Data_Eeprom(unsigned char EepromAddress, unsigned char EepromData)
{
    CLRWDT();
//  GIE = 0;
    EEADR = EepromAddress;
    asm("nop");
    EEDAT = EepromData;
    EECON1 = 0x34;
    WR = 1;
    asm("nop"); 
    asm("nop");
    asm("nop");
    while(WR == 1) CLRWDT();
    WREN1 = 0;
    WREN2 = 0;
    WREN3 = 0;
//  GIE = 1;
}

void Device_Initial(void)
{
   OSCCON = 0b01010000;      //=(1/4MHz)*4=1us
   //WPD = 0b00000000;
   //ANSEL = 0b00000000;
   PORTA = 0b00000000;
   TRISA = 0b00001100;
    asm("nop");
}
 void ms_delay(int value)
 {
    int a;
    for(a = 0; a < value; a++)
    {
        CLRWDT();
      __delay_ms(1); 
    } 
 }
void RUN()
{
  LED1=Read_8bit_Data_Eeprom(0x00);
  LED2=Read_8bit_Data_Eeprom(0x10);
  if(LED1==0&&LED2==1)
  {   i=1001;
      while(i>1000)
          {
              LED2=1;
              LED1=0;
          if(SW2==1)
            {
             LED1=1;
             int m=0;
             while(SW2==1&&i)
             {
              __delay_ms(10);
              m++;
              if(m>100)
             {
              i=0;
              if(SW2==1)
              {LED1=1;
              __delay_ms(100);
              LED1=0;
              __delay_ms(100);
              LED1=1;
              LED2=1;
			  num  =3;
              }
              else{LED2=0;}
             }
            }
         }else{LED1=0;} 
         
        }
      }
          while(1)
          {

          int i=0,m=0;
          if(SW1==1)
          {
           LED2=1;
          }
          else
          {
          if(SW2==1)
          {
          LED2=1;
          __delay_ms(10);
          while(SW2==1)
          {
              LED2=1;
              if(i>1000)
            {  
            LED1=0;         
            LED2=1;
            }
              __delay_ms(1);
              i++;
          }
          Write_8bit_Data_Eeprom(0x00,LED1);
          Write_8bit_Data_Eeprom(0x10,LED2);
          T:while(i>1000)
          {
              LED2=1;
              LED1=0;
          if(SW2==1)
            {
             LED1=1;
             m=0;
             while(SW2==1&&i)
             {
              __delay_ms(10);
              m++;
              if(m>100)
             {
              i=0;
              if(SW2==1)
              {LED1=1;
              __delay_ms(100);
              LED1=0;
              __delay_ms(100);
              LED1=1;
              LED2=1;
			  num  =3;
              }
              else{LED2=0;}
             }
            }
         }else{LED1=0;} 
         
        };
         }
          else
          {
              LED2=0;
          }
          }

}
}

void main(void)
{
    Device_Initial();
    LED1=1;
    LED2=0;
	RUN();

}