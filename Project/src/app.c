#include "app.h"

/********************************************//**
 *   User variables
 ***********************************************/
static int xdata t_hour_begin, t_minute_begin, t_second_begin;
static int xdata t_hour_end, t_minute_end, t_second_end;

static RTC_time_t xdata rtc_time;
static RTC_date_t xdata rtc_date;

volatile uint16_t tick = 0;
volatile uint8_t request_ds1307_interval = LOW;

static STATE_t state = NORMAL;
static uint8_t timer_flag = FLOATNG;
static uint8_t timer_mode = FLOATNG;

char xdata payLoad[PAYLOAD_LEN];
unsigned char xdata index_data = 0;
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
	
	// Initialize DS1307
	DS1307_Init();
	
	// Load flash
	check_btn_last_state();
	check_timer();
	
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

void check_button(void)
{
	uint16_t timeout = 0, last_state;
	
	if (timer_flag == HIGH)
	{
		if (state == NORMAL)
		{
			if(BTN2_PAS == HIGH)
			{
				LED_RELAY2 = HIGH;
				Delay_ms(10);
				while (BTN2_PAS == HIGH)
				{
					if(timeout == 2000)
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
					if(timeout == 2000)
					{  
						LED_RELAY2 = LOW;
							
						LED_RELAY1 = HIGH;
						Delay_ms(100);
						LED_RELAY1 = LOW;
						Delay_ms(100);
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

void check_timer(void)
{
	t_hour_begin     = Read_APROM_BYTE(DS1307_HOURS_START_ADDRESS);
	t_minute_begin   = Read_APROM_BYTE(DS1307_MINUTE_START_ADDRESS);
	t_second_begin   = Read_APROM_BYTE(DS1307_SECOND_START_ADDRESS);
	
  t_hour_end    = Read_APROM_BYTE(DS1307_HOURS_STOP_ADDRESS);
	t_minute_end  = Read_APROM_BYTE(DS1307_MINUTE_STOP_ADDRESS);
	t_second_end  = Read_APROM_BYTE(DS1307_SECOND_STOP_ADDRESS);
	
  timer_mode  = Read_APROM_BYTE(DS1307_TIME_MODE_FLAG);	
	timer_flag  = Read_APROM_BYTE(DS1307_TIMER_FLAG);
	
	if(timer_flag == HIGH)
	{
		LED_RELAY1 = HIGH;
		LED_RELAY2 = LOW;
	}
	else
	{
		LED_RELAY1 = LOW;
		LED_RELAY2 = LOW;
	}
}

void check_uart(void)
{
	char xdata header[3], hours[3], minutes[3], seconds[3];
	
	if (IsData() == '#')
	{
		// read from message header
		while(IsAvailable())
		{
			if (uartReadByte() == '$') 
			{
				payLoad[index_data++] = '$';
				break;
			}
		}
		
		// read the message body
		while(IsAvailable())
		{
			payLoad[index_data++] = uartReadByte();
		}
		payLoad[index_data] = '\0';
		// clear buffer
		uartResetNByte(PAYLOAD_LEN);
		
		// parser data
		parser_data(header, hours, minutes, seconds, payLoad);
		
		if(!strcmp(header, "$C"))
    {
			rtc_time.hours       = atoi(hours);
			rtc_time.minutes     = atoi(minutes);
			rtc_time.seconds     = atoi(seconds);
			DS1307_Set_Current_Time(&rtc_time);
	
			printf("\nOK\n");
		}
		else if (!strcmp(header, "$S")) // set time relay 1 on
		{
			t_hour_begin     = atoi(hours);
			t_minute_begin   = atoi(minutes);
			t_second_begin   = atoi(seconds);
	
			printf("\nOK\n");
			Write_DATAFLASH_BYTE(DS1307_HOURS_START_ADDRESS, t_hour_begin);
			Write_DATAFLASH_BYTE(DS1307_MINUTE_START_ADDRESS, t_minute_begin);
			Write_DATAFLASH_BYTE(DS1307_SECOND_START_ADDRESS, t_second_begin);			
		}
		else if (!strcmp(header, "$E")) // set time relay 1 off
		{
			t_hour_end     = atoi(hours);
			t_minute_end   = atoi(minutes);
			t_second_end   = atoi(seconds);
	
			printf("\nOK\n");
			Write_DATAFLASH_BYTE(DS1307_HOURS_STOP_ADDRESS, t_hour_end);
			Write_DATAFLASH_BYTE(DS1307_MINUTE_STOP_ADDRESS, t_minute_end);
			Write_DATAFLASH_BYTE(DS1307_SECOND_STOP_ADDRESS, t_second_end);			
			
			timer_flag = LOW;
		}
		else
		{
			// do something here
		}
		
		// clear buffer
		index_data = 0;
		memset(payLoad, 0, PAYLOAD_LEN);
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

void ds1307(void)
{
	if(request_ds1307_interval == HIGH)
	{
		// get current time
		DS1307_Get_Current_Time(&rtc_time);
		
		// get current date
		DS1307_Get_Current_Date(&rtc_date);
		
		if(timer_flag == LOW)
		{
			if(TIME2SECOND(t_hour_begin, t_minute_begin, t_second_begin) == TIME2SECOND(t_hour_end,t_minute_end, t_second_end))
			{
				timer_flag = FLOATNG;
				Write_DATAFLASH_BYTE(DS1307_TIMER_FLAG, timer_flag);
				
				printf("\SET TIMER ERROR\n");
			}
			
		  // relay 1 on and relay 2 off => 6.am - 17.pm
			if(TIME2SECOND(t_hour_begin, t_minute_begin, t_second_begin) < TIME2SECOND(t_hour_end, t_minute_end, t_second_end))
			{
				printf("1\n");
				if((TIME2SECOND(rtc_time.hours, rtc_time.minutes, rtc_time.seconds) > TIME2SECOND(t_hour_begin, t_minute_begin, t_second_begin)) && 
					 (TIME2SECOND(rtc_time.hours, rtc_time.minutes, rtc_time.seconds) < TIME2SECOND(t_hour_end,t_minute_end, t_second_end)))
				{
					LED_RELAY1 = HIGH;
					LED_RELAY2 = LOW;
					
					timer_flag = HIGH;
					timer_mode = LINE_TIME;
					
					Write_DATAFLASH_BYTE(DS1307_TIMER_FLAG, timer_flag);
					Write_DATAFLASH_BYTE(DS1307_TIME_MODE_FLAG, timer_mode);
					
					printf("\n Jump into active mode 1\n");
				}
			}
			
			// relay 1 on and relay 2 off => 19.pm - 6.am
			if(TIME2SECOND(t_hour_begin, t_minute_begin, t_second_begin) > TIME2SECOND(t_hour_end,t_minute_end, t_second_end))
			{
				printf("2\n");
				if(TIME2SECOND(rtc_time.hours, rtc_time.minutes, rtc_time.seconds) > TIME2SECOND(t_hour_begin, t_minute_begin, t_second_begin))
				{
					LED_RELAY1 = HIGH;
					LED_RELAY2 = LOW;
					
					timer_flag = HIGH;
					timer_mode = WRAP_TIME;
					
					Write_DATAFLASH_BYTE(DS1307_TIMER_FLAG, timer_flag);
					Write_DATAFLASH_BYTE(DS1307_TIME_MODE_FLAG, timer_mode);
		
					printf("\n Jump into active mode 2\n");
				}	
			}
		}
		
		if(timer_flag == HIGH)
		{
			// relay 1 and relay 2 off => 6.am - 17.pm
			if((TIME2SECOND(t_hour_begin, t_minute_begin, t_second_begin) < TIME2SECOND(t_hour_end,t_minute_end, t_second_end)) &&
				 (timer_mode == LINE_TIME))
			{
				printf("12\n");
				if(TIME2SECOND(rtc_time.hours, rtc_time.minutes, rtc_time.seconds) < TIME2SECOND(t_hour_begin, t_minute_begin, t_second_begin))
				{
					LED_RELAY1 = LOW;
					LED_RELAY2 = LOW;
					
					timer_flag = LOW;
					timer_mode = FLOATNG;
					
					Write_DATAFLASH_BYTE(DS1307_TIMER_FLAG, timer_flag);
					Write_DATAFLASH_BYTE(DS1307_TIME_MODE_FLAG, timer_mode);
					
					printf("\n Jump out active mode 1\n");
				}
				
				if(TIME2SECOND(rtc_time.hours, rtc_time.minutes, rtc_time.seconds) > TIME2SECOND(t_hour_end,t_minute_end, t_second_end))
				{
					LED_RELAY1 = LOW;
					LED_RELAY2 = LOW;
					
					timer_flag = LOW;
					timer_mode = FLOATNG;
					Write_DATAFLASH_BYTE(DS1307_TIMER_FLAG, timer_flag);
					Write_DATAFLASH_BYTE(DS1307_TIME_MODE_FLAG, timer_mode);
					
					printf("\n Jump out active mode 1\n");
				}
			}
			
			// relay 1 and relay 2 off => 19.pm - 6.am
			if((TIME2SECOND(t_hour_begin, t_minute_begin, t_second_begin) > TIME2SECOND(t_hour_end,t_minute_end, t_second_end)) &&
				 (timer_mode == WRAP_TIME))
			{
				printf("21\n");
							
				if((TIME2SECOND(rtc_time.hours, rtc_time.minutes, rtc_time.seconds) < TIME2SECOND(t_hour_begin, t_minute_begin, t_second_begin)) && // 6.am -> 19.pm
					 (TIME2SECOND(rtc_time.hours, rtc_time.minutes, rtc_time.seconds) > TIME2SECOND(t_hour_end,t_minute_end, t_second_end)))
				{
					LED_RELAY1 = LOW;
					LED_RELAY2 = LOW;
					
					timer_flag = LOW;
					timer_mode = FLOATNG;
					
					Write_DATAFLASH_BYTE(DS1307_TIMER_FLAG, timer_flag);
					Write_DATAFLASH_BYTE(DS1307_TIME_MODE_FLAG, timer_mode);
					
					printf("\n Jump out active mode 2\n");
				}					
			}
		}
		
		request_ds1307_interval = LOW;
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
