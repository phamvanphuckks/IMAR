/* ds1307.h */
#ifndef DS1307_H_
#define DS1307_H_

#define DS1307_SECONDS    0x00
#define DS1307_MINUTES    0x01
#define DS1307_HOURS      0x02
#define DS1307_DAY        0x03
#define DS1307_DATE       0x04
#define DS1307_MONTH      0x05
#define DS1307_YEAR       0x06

#define TIME_FORMAT_12HRS_AM       0x00
#define TIME_FORMAT_12HRS_PM       0x01
#define TIME_FORMAT_24HRS          0x02

#define DS1307_ADDRESS_W  0xD0
#define DS1307_ADDRESS_R  0xD1

#define BCD2DEC(num)    (((num)/16 * 10) + ((num) % 16))
#define DEC2BCD(num)    (((num)/10 * 16) + ((num) % 10))

enum {
    SUNDAY = 1,
	  MONDAY,
	  TUESDAY,
	  WEDNESDAY,
	  THURSDAY,
	  FIRDAY,
	  SATURDAY
};

typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t time_format;
} RTC_time_t;

typedef struct
{
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
} RTC_date_t;

void DS1307_Init(void);
void DS1307_Set_Current_Time(RTC_time_t *time);
void DS1307_Set_Current_Date(RTC_date_t *date);
void DS1307_Get_Current_Time(RTC_time_t *rtc_time);
void DS1307_Get_Current_Date(RTC_date_t *rtc_date);
char * time2string(RTC_time_t *time);
char * date2string(RTC_date_t *rtc_date);

// I2C interface
uint8_t DS1307_Write(uint8_t Address, uint8_t *pData, uint8_t length);
uint8_t DS1307_Read(uint8_t Address, uint8_t *pData, uint8_t length);
uint8_t DS1307_CheckAddress(void);

// somethings
uint8_t dec2bcd(uint8_t value);
uint8_t bcd2dec(uint8_t value);

#endif
