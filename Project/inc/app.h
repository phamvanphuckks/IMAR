#ifndef __APP_H__
#define __APP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hw_config.h"
#include "MS51_16K.H"
#include "delay.h"
#include "gpio.h"
#include "uart.h"
#include "timerx.h"
#include "ds1307.h"

typedef enum STATUS {
	ON  = 1,
	OFF = !ON
} stt_t;

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
void ds1307(void);
void check_uart(void);
void parser_data(char *header, char *hours, char *minutes, char *second, char *payload);

#endif
