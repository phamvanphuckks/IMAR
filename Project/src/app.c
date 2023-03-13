#include "app.h"

/********************************************//**
 *   User variables
 ***********************************************/
typedef enum {
	NORMAL = 1,
	PASS_HOLD_3S,
	PASS_ALARM_5S
} STATE_t;

typedef enum {
	LOW = 0,
	HIGH,
}; 
 
static int xdata set_second, set_minute, set_hour, day, wday, month, year;
static RTC_time_t xdata rtc_time;
static RTC_date_t xdata rtc_date;
volatile uint16_t tick;
volatile uint8_t request_ds1307_interval = LOW;

static STATE_t state = NORMAL;
static state_alram = 0;

char xdata payLoad[13];
unsigned char index_data = 0;
/********************************************//**
 *   User functions
 ***********************************************/
void app_init(void)
{
	MODIFY_HIRC(HIRC_16);
	
	// Initialize GPIO
	GPIO_init();
	
	// Initialize UART
	uart_init();
	
	// Initialize delay - timer0
	Delay_init();
	
	/* init */
	DS1307_Init();
	
	rtc_date.day   = MONDAY;
	rtc_date.date  = 0;
	rtc_date.month = 0;
	rtc_date.year  = 0;

  rtc_time.hours       = 0;
	rtc_time.minutes     = 0;
	rtc_time.seconds     = 0;
	rtc_time.time_format = TIME_FORMAT_24HRS;
  DS1307_Set_Current_Time(&rtc_time);
	DS1307_Set_Current_Time(&rtc_date);
	
	check_btn_last_state();

	// TIMER1 Init 
	TIM1_init();
}

void app_process_action(void)
{
	check_button();

	check_uart();

	ds1307();
}

void uart_init(void)
{
	UART_Open(FREQ_SYS, UART0_Timer3, 9600);
	ENABLE_UART0_PRINTF;
  ENABLE_UART0_INTERRUPT;   /* Enable UART0 interrupt */
  ENABLE_GLOBAL_INTERRUPT;  /* Global interrupt enable */
	
	uartFresh();
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

void check_uart(void)
{
	char xdata header[3], hours[3], minutes[3], seconds[3];
	
	if (IsData() == '#')
	{
		while(IsAvailable())
		{
			payLoad[index_data++] = uartReadByte();
		}
		payLoad[index_data] = '\0';
		// clear buffer
		uartResetNByte();
		
		// parser data
		parser_data(header, hours, minutes, seconds, payLoad);
		
		if(!strcmp(header, "$C"))
    {
			rtc_time.hours       = atoi(hours);
			rtc_time.minutes     = atoi(minutes);
			rtc_time.seconds     = atoi(seconds);
			DS1307_Set_Current_Time(&rtc_time);
			UART_Send_Data(UART0, 'C');
		}
		else if (!strcmp(header, "$S"))
		{
			set_second = atoi(seconds);
			set_minute = atoi(minutes);
			set_hour = atoi(hours);
			
			state_alram = 1;
			Delay_ms(10);
			UART_Send_Data(UART0, 'S');
		}
		else if (!strcmp(header, "$E"))
		{
			
			UART_Send_Data(UART0, 'E');
		}
		else
		{
			
		}
		
		// clear buffer
		index_data = 0;
		memset(payLoad, 0, 13);
	}
}

void ds1307(void)
{
	if((request_ds1307_interval == HIGH) && (state_alram == 1))
	{
		DS1307_Get_Current_Time(&rtc_time);
		UART_Send_Data(UART0, 'I');
		Delay_ms(10);
		if(rtc_time.hours >= set_hour)
		{
			UART_Send_Data(UART0, 'X');
			Delay_ms(10);
			if(rtc_time.minutes >= set_minute)
			{
				UART_Send_Data(UART0, 'Y');
				Delay_ms(10);
				if(rtc_time.seconds >= set_second)
				{
					LED_RELAY1 = LOW;         
					LED_RELAY2 = LOW;
					state_alram = 0;
					
					
					UART_Send_Data(UART0, 'Z');
					Delay_ms(10);
				}
			}
		}
		
		request_ds1307_interval = LOW;
	}
}

void parser_data(char *header, char *hours, char *minutes, char *seconds, char *payLoad)
{
	char *p_rx, *p_header, *p_hours, *p_minutes, *p_seconds;
	
	// parser data => $C,23,17,05# - Set current time || $S,23,17,05# - Alarm on || $E,23,17,05# - Alarm off
  if((header != NULL) && (hours != NULL) && (minutes != NULL) && (seconds != NULL) && (payLoad != NULL))
  {	
		p_rx = payLoad;
		p_header = p_rx;
		p_rx = strchr(p_header, ',');
		if(p_rx != NULL)
		{
			*p_rx = 0;
			strcpy(header, p_header);
		}		
		
		p_hours = p_rx+1;
		p_rx = strchr(p_hours, ',');
		if(p_rx != NULL)
		{
				*p_rx = 0;
				strcpy(hours, p_hours);
		}
		
		p_minutes = p_rx+1;
		p_rx = strchr(p_minutes, ',');
		if(p_rx != NULL)
		{
				*p_rx = 0;
				strcpy(minutes, p_minutes);
		}
		
		p_seconds = p_rx+1;
		p_rx = strchr(p_seconds, '#');
		if(p_rx != NULL)
		{
				*p_rx = 0;
				strcpy(seconds, p_seconds);
		}
	}
}

/************************************************************************************************************
     Timer1 interrupt subroutine
************************************************************************************************************/
void Timer1_ISR (void) interrupt 3
{
  _push_(SFRS);

	tick++;
	if (tick >= 1000) // for 1s
	{
		request_ds1307_interval = HIGH;
		tick = 0;
	}
	
	TH1 = TH1_INIT;
  TL1 = TL1_INIT;
	TF0 = 0;
	
  _pop_(SFRS);
}
