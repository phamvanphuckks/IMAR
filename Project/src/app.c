#include "app.h"

/********************************************//**
 *   User variables
 ***********************************************/
int xdata second, minute, hour, day, wday, month, year;
RTC_time_t xdata rtc_time;
RTC_date_t xdata rtc_date;

typedef enum {
	NORMAL = 1,
	PASS_HOLD_3S,
	PASS_ALARM_5S
} STATE_t;

typedef enum {
	LOW = 0,
	HIGH,
};

static STATE_t state = NORMAL;

/********************************************//**
 *   User functions
 ***********************************************/
void app_init(void)
{
	MODIFY_HIRC(HIRC_24);
	
	// Initialize GPIO
	GPIO_init();
	
	// Initialize UART
	uart_init();
	
	// Initialize delay - timer0
	Delay_init();
	
	/* init */
	DS1307_Init();
	
	rtc_date.day = MONDAY;
	rtc_date.date = 0;
	rtc_date.month = 0;
	rtc_date.year = 0;

  rtc_time.hours = 0;
	rtc_time.minutes = 0;
	rtc_time.seconds = 0;
	rtc_time.time_format = TIME_FORMAT_24HRS;
  DS1307_Set_Current_Time(&rtc_time);
	DS1307_Set_Current_Time(&rtc_date);
	
	check_btn_last_state();
}

void app_process_action(void)
{
	 check_button();
	
   ds1307();
}

void uart_init(void)
{
	UART_Open(FREQ_SYS, UART0_Timer3, 9600);
	ENABLE_UART0_PRINTF;
  ENABLE_UART0_INTERRUPT;
  ENABLE_GLOBAL_INTERRUPT;  //ENABLE_GLOBAL_INTERRUPT
}

static void check_button(void)
{
	uint16_t timeout = 0, last_state;
	
  if (state == NORMAL)
	{
		if(BTN2_PAS == HIGH)
		{
			LED_RELAY2 = HIGH;
			Delay_ms(10);
			while (BTN2_PAS == HIGH)
			{
			  if(timeout == 3000)
				{  
					LED_RELAY1 = LOW;         
					LED_RELAY2 = HIGH;
					state = PASS_HOLD_3S;
					
					last_state = Read_APROM_BYTE(STATE_MODE);
					if( last_state != PASS_HOLD_3S)
					{
						Write_DATAFLASH_BYTE(STATE_MODE, state);
					}
				}
				Delay_ms(1);
				timeout++;
			}
		}
		else 
		{
			LED_RELAY2 = (BTN1_COS == HIGH)? HIGH : LOW;
		}
	}
	
  if (state == PASS_HOLD_3S)
	{
		if(BTN2_PAS == HIGH)
		{
			LED_RELAY1 = HIGH;
			Delay_ms(10);
			
			timeout = 0;
			while (BTN2_PAS == HIGH)
			{
			  if(timeout == 3000)
				{  
				  LED_RELAY2 = LOW;
						
					LED_RELAY1 = HIGH;
          Delay_ms(300);
          LED_RELAY1 = LOW;
          Delay_ms(300);
					LED_RELAY1 = HIGH;
						
					state = NORMAL;
					
					last_state = Read_APROM_BYTE(STATE_MODE);
					if( last_state != NORMAL)
					{
						Write_DATAFLASH_BYTE(STATE_MODE, state);
					}
			  }
				Delay_ms(1);
				timeout++;
		  }
		}
		else
		{
			LED_RELAY1 = 0;
		}
	}
}

void check_btn_last_state(void)
{ 	
	uint8_t last_state;
	
	last_state = Read_APROM_BYTE(STATE_MODE);
	if(last_state != 0xFF)
	{
		state = last_state;
	}
	
	if(state == NORMAL)
	{
	  LED_RELAY1 = 1;
    LED_RELAY2 = 0;		
	} 
	else if (state == PASS_HOLD_3S)
	{
	  LED_RELAY1 = 0;
    LED_RELAY2 = 1;		
	}
	else {
		LED_RELAY1 = 1;
    LED_RELAY2 = 0;
	}
}

void ds1307(void)
{

}
