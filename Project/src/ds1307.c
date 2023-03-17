/* ds1307.c */

#include "MS51_16K.H"
#include "ds1307.h"

static uint8_t send_stop(void);

void DS1307_Init(void)
{
	I2CLK = 39;
	/* P1.3 */
	/* Quasi */
	P1M1 &= ~(1 << 3);
	P1M2 &= ~(1 << 3);
	/* P1.4 */
	/* Quasi */
	P1M1 &= ~(1 << 4);
	P1M2 &= ~(1 << 4);
	
	P13 = 1;
	P14 = 1;
	
	I2CEN = 1;
}

uint8_t DS1307_Write(uint8_t Address, uint8_t *pData, uint8_t length)
{
	uint8_t i;
	uint16_t t;
	uint8_t u8TimeOut;
	
	if (I2STAT != 0xF8) {
		return 0;
	}
	
	/* start */
	STO = 0;
	STA = 1;
	SI = 0;
	t = 1;
	u8TimeOut = 0;
	
	while (1) {
		if (SI) {
			break;
		}
		if (!t) {
			u8TimeOut = 1;
			break;
		}
		++t;
	}
	
	if (u8TimeOut) {
		/* start error */
		send_stop();
		return 0;
	}
	
	if (I2STAT != 0x08) {
		/* start error */
		send_stop();
		return 0;
	}
	
	/* send DS1307 address */
	STA = 0;
	STO = 0;
	I2DAT = DS1307_ADDRESS_W;
	SI = 0;
	t = 1;
	u8TimeOut = 0;
	while (1) {
		if (SI) {
			break;
		}
		if (!t) {
			u8TimeOut = 1;
			break;
		}
		++t;
	}
	
	if (u8TimeOut) {
		/* send address error */
		send_stop();
		return 0;
	}
	
	if (I2STAT != 0x18) {
		/* send address error */
		send_stop();
		return 0;
	}
	
	/* send address */
	STA = 0;
	STO = 0;
	I2DAT = Address;
	SI = 0;
	t = 1;
	u8TimeOut = 0;
	while (1) {
		if (SI) {
			break;
		}
		if (!t) {
			u8TimeOut = 1;
			break;
		}
		++t;
	}
	
	if (u8TimeOut) {
		/* send address error */
		send_stop();
		return 0;
	}
	
	if (I2STAT != 0x28) {
		/* send address error */
		send_stop();
		return 0;
	}
	
	/* send data */
	for (i = 0; i < length; ++i) {
		I2DAT = pData[i];
		SI = 0;
		t = 1;
		u8TimeOut = 0;
		while (1) {
			if (SI) {
				break;
			}
			if (!t) {
				u8TimeOut = 1;
				break;
			}
			++t;
		}
		if (u8TimeOut) {
			/* send data error */
			send_stop();
			return 0;
		}
		if (I2STAT != 0x28) {
			/* send data error */
			send_stop();
			return 0;
		}
	}
	
	/* stop */
	return send_stop();
}

uint8_t DS1307_Read(uint8_t Address, uint8_t *pData, uint8_t length)
{
	uint8_t i;
	uint16_t t;
	uint8_t u8TimeOut;
	
	if (I2STAT != 0xF8) {
		return 0;
	}
	
	/* start */
	STO = 0;
	STA = 1;
	SI = 0;
	t = 1;
	u8TimeOut = 0;
	
	while (1) {
		if (SI) {
			break;
		}
		if (!t) {
			u8TimeOut = 1;
			break;
		}
		++t;
	}
	
	if (u8TimeOut) {
		/* start error */
		send_stop();
		return 0;
	}
	
	if (I2STAT != 0x08) {
		/* start error */
		send_stop();
		return 0;
	}
	
	/* send DS1307 address */
	STA = 0;
	STO = 0;
	I2DAT = DS1307_ADDRESS_W;
	SI = 0;
	t = 1;
	u8TimeOut = 0;
	while (1) {
		if (SI) {
			break;
		}
		if (!t) {
			u8TimeOut = 1;
			break;
		}
		++t;
	}
	
	if (u8TimeOut) {
		/* send address error */
		send_stop();
		return 0;
	}
	
	if (I2STAT != 0x18) {
		/* send address error */
		send_stop();
		return 0;
	}
	
	/* send address */
	STA = 0;
	STO = 0;
	I2DAT = Address;
	SI = 0;
	t = 1;
	u8TimeOut = 0;
	while (1) {
		if (SI) {
			break;
		}
		if (!t) {
			u8TimeOut = 1;
			break;
		}
		++t;
	}
	
	if (u8TimeOut) {
		/* send address error */
		send_stop();
		return 0;
	}
	
	if (I2STAT != 0x28) {
		/* send address error */
		send_stop();
		return 0;
	}
	
	/* start */
	STO = 0;
	STA = 1;
	SI = 0;
	t = 1;
	u8TimeOut = 0;
	
	while (1) {
		if (SI) {
			break;
		}
		if (!t) {
			u8TimeOut = 1;
			break;
		}
		++t;
	}
	
	if (u8TimeOut) {
		/* start error */
		send_stop();
		return 0;
	}
	
	if (I2STAT != 0x10) {
		/* start error */
		send_stop();
		return 0;
	}
	
	/* send DS1307 address */
	STA = 0;
	STO = 0;
	I2DAT = DS1307_ADDRESS_R;
	SI = 0;
	t = 1;
	u8TimeOut = 0;
	while (1) {
		if (SI) {
			break;
		}
		if (!t) {
			u8TimeOut = 1;
			break;
		}
		++t;
	}
	
	if (u8TimeOut) {
		/* send address error */
		send_stop();
		return 0;
	}
	
	if (I2STAT != 0x40) {
		/* send address error */
		send_stop();
		return 0;
	}
	
	/* get data */
	for (i = 0; i < length - 1; ++i) {
		AA = 1;
		SI = 0;
		t = 1;
		u8TimeOut = 0;
		while (1) {
			if (SI) {
				break;
			}
			if (!t) {
				u8TimeOut = 1;
				break;
			}
			++t;
		}
		if (u8TimeOut) {
			/* send data error */
			send_stop();
			return 0;
		}
		if (I2STAT != 0x50) {
			/* send data error */
			send_stop();
			return 0;
		}
		pData[i] = I2DAT;
	}
	
	/* last byte */
	AA = 0;
	SI = 0;
	t = 1;
	u8TimeOut = 0;
	while (1) {
		if (SI) {
			break;
		}
		if (!t) {
			u8TimeOut = 1;
			break;
		}
		++t;
	}
	if (u8TimeOut) {
		/* send data error */
		send_stop();
		return 0;
	}
	if (I2STAT != 0x58) {
		/* send data error */
		send_stop();
		return 0;
	}
	pData[i] = I2DAT;
	/* stop */
	return send_stop();
}

uint8_t DS1307_CheckAddress(void)
{
	uint16_t t;
	uint8_t u8TimeOut;
	
	if (I2STAT != 0xF8) {
		return 0;
	}
	
	/* start */
	STO = 0;
	STA = 1;
	SI = 0;
	t = 1;
	u8TimeOut = 0;
	
	while (1) {
		if (SI) {
			break;
		}
		if (!t) {
			u8TimeOut = 1;
			break;
		}
		++t;
	}
	
	if (u8TimeOut) {
		/* start error */
		send_stop();
		return 0;
	}
	
	if (I2STAT != 0x08) {
		/* start error */
		send_stop();
		return 0;
	}
	
	/* send address */
	STA = 0;
	STO = 0;
	I2DAT = DS1307_ADDRESS_R;
	SI = 0;
	t = 1;
	u8TimeOut = 0;
	while (1) {
		if (SI) {
			break;
		}
		if (!t) {
			u8TimeOut = 1;
			break;
		}
		++t;
	}
	
	if (u8TimeOut) {
		/* send address error */
		send_stop();
		return 0;
	}
	
	if (I2STAT != 0x40) {
		/* send address error */
		send_stop();
		return 0;
	}
	
	/* last byte */
	AA = 0;
	SI = 0;
	t = 1;
	u8TimeOut = 0;
	while (1) {
		if (SI) {
			break;
		}
		if (!t) {
			u8TimeOut = 1;
			break;
		}
		++t;
	}
	if (u8TimeOut) {
		/* send data error */
		send_stop();
		return 0;
	}
	if (I2STAT != 0x58) {
		/* send data error */
		send_stop();
		return 0;
	}
	t = I2DAT;
	/* stop */
	return send_stop();
}

uint8_t send_stop(void)
{
	uint16_t t;
	uint8_t u8TimeOut;
	
	STA = 0;
	STO = 1;
	SI = 0;
	t = 1;
	u8TimeOut = 0;
	while (1) {
		if (I2STAT == 0xF8) {
			break;
		}
		if (!t) {
			u8TimeOut = 1;
			break;
		}
		++t;
	}
	return (!u8TimeOut);
}

uint8_t dec2bcd(uint8_t value)
{
	uint8_t m, n;
	uint8_t bcd;
	
	bcd = value;
	if (value >= 10)
	{
		m = value/10;
		n = value%10;
		bcd = (m << 4) | n;
	}
	
	return bcd;
}

uint8_t bcd2dec(uint8_t value)
{
	uint8_t m, n;
	
	m = (uint8_t)((value >> 4) * 10);
	n = value & (uint8_t)0x0F;

	return (m + n);
}

void DS1307_Set_Current_Time(RTC_time_t *rtc_time)
{
	uint8_t hours, minutes, seconds;
	
	seconds = dec2bcd(rtc_time->seconds);
	seconds &= ~(1 << 7);
	DS1307_Write(DS1307_SECONDS, &seconds, 1);
	
	minutes = dec2bcd(rtc_time->minutes);
	DS1307_Write(DS1307_MINUTES, &minutes, 1);
	
	hours = dec2bcd(rtc_time->hours);
	if(rtc_time->time_format == TIME_FORMAT_24HRS) 
	{
		hours &= ~(1 << 6);
	}
	else
	{
		hours |= ~(1 << 6);
		
		if(rtc_time->time_format == TIME_FORMAT_12HRS_PM)
		{
			hours |= (1 << 5);
		}
		else 
		{
			hours &= ~(1 << 5);
		}
	}
	
	DS1307_Write(DS1307_HOURS, &hours, 1);
}

void DS1307_Set_Current_Date(RTC_date_t *rtc_date)
{
	uint8_t day, date, month, year;
	
	day = dec2bcd(rtc_date->day);
	DS1307_Write(DS1307_DAY, &day, 1);
	
	date = dec2bcd(rtc_date->date);
	DS1307_Write(DS1307_DATE, &date, 1);
	
	month = dec2bcd(rtc_date->month);
	DS1307_Write(DS1307_MONTH, &month, 1);
	
	year = dec2bcd(rtc_date->year);
	DS1307_Write(DS1307_YEAR, &year, 1);
}

void DS1307_Get_Current_Time(RTC_time_t *rtc_time)
{
	uint8_t hours, minutes, seconds;
	
	DS1307_Read(DS1307_SECONDS, &seconds, 1);
	seconds &=  ~(1 << 7);
	rtc_time->seconds = bcd2dec(seconds);
	
	DS1307_Read(DS1307_MINUTES, &minutes, 1);
	rtc_time->minutes = bcd2dec(minutes);
	
	DS1307_Read(DS1307_HOURS, &hours, 1);
	if( hours & (1 << 6))
	{
		// 12 hour format
		rtc_time->time_format = !((hours & (1 << 5)) == 0);
		hours &= ~(0x03 << 5); // bit 5, 6
	}
	else 
	{
		rtc_time->time_format = TIME_FORMAT_24HRS;
	}
	rtc_time->hours = bcd2dec(hours);
}

void DS1307_Get_Current_Date(RTC_date_t *rtc_date)
{
	uint8_t day, date, month, year;
	
	DS1307_Read(DS1307_DAY, &day, 1);
	rtc_date->day = bcd2dec(day);
	
	DS1307_Read(DS1307_DATE, &date, 1);
	rtc_date->date = bcd2dec(date);
	
	DS1307_Read(DS1307_MONTH, &month, 1);
	rtc_date->month = bcd2dec(month);
	
	DS1307_Read(DS1307_YEAR, &year, 1);
	rtc_date->year = bcd2dec(year);
}
