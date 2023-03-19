#ifndef __APP_H__
#define __APP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MS51_16K.H"
#include "hw_config.h"
#include "delay.h"
#include "gpio.h"
#include "uart.h"
#include "timerx.h"
#include "ds1307.h"

#define PAYLOAD_LEN          13
#define LOG_DEBUG            0

// MACRO to convert to second unit
#define TIME2SECOND(h, m, s) (long)((long)(h)*3600 + (long)(m)*60 + (long)(s))

typedef enum STATUS {
	OFF = 0,
	ON,
	FLOATNG,
	WRAP_TIME,
	LINE_TIME
} stt_t;

typedef enum {
	NORMAL = 1,
	PASS_HOLD_3S,
	ALARM_START,
	ALARM_STOP
} STATE_t;

typedef enum {
	LOW = 0,
	HIGH,
};

typedef enum {
  /** Success return value - no error occurred */
  SUCCESS = 0,
  /** A generic error. Not enough information for a specific error code */
  FAILURE = -1,
  /** A required parameter was passed as null */
  NULL_VALUE_ERROR = -2,
  /** A value get our of range*/
  INVALID_VALUE_RANGE = -3,
	/** Timeout*/
	TIMEOUT = -4,
} error_t;

/*
 *  @brief
 *      App ticking function.
 *  @param
 *      None.
 *  @return
 *      None.
 */
void app_process_action(void);

/*
 *  @brief
 *       Initialize application.
 *  @param
 *       None.
 *  @return
 *       None.
 */
void app_init(void);

void uart_init(void);
void check_button(void);
void check_btn_last_state(void);
void check_timer(void);
void check_ds1307(void);
void check_uart(void);
void parser_data(char *header, char *hours, char *minutes, char *seconds, char *payLoad);

#endif
