#pragma once

#include <stdio.h>
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_ble_mesh_networking_api.h"

#include "../RD_Peripheral/rd_gpio.h"
#include "../RD_lib.h"
#include "LC8823.h"

#define	X1_TOUCH_SWITCH				1
#define	X2_TOUCH_SWITCH				2
#define	X3_TOUCH_SWITCH				3
#define	X4_TOUCH_SWITCH				4

#define	TYPE_HARDWARE_SWITCH		X4_TOUCH_SWITCH

#define TIME_CYCLE_SCAN_BTN         5000 //us
#define COUNT_CYCLE_ACTIVE_BTN      5
#define TIME_CHECK_KICK_OUT         3*1000*1000  //us
#define TIME_OUT_KICK_OUT           10*1000*1000
typedef enum
{
	But_None	= 0x00U,
	But_Press	= 0x01U,
	But_Keeping	= 0x02U
} Button_Stt_Type_t;

void rd_scan_btn(void);
void rd_scan_btn_v2(void);
void check_time_kick_out(void);
