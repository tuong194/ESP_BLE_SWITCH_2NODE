#pragma once

#include <stdio.h>
#include "driver/gpio.h"
#include "../RD_lib.h"

/*18, 19: D+ D-
IO9: boot
20,21: tx rx*/

#define RESET_TOUCH_PIN  GPIO_NUM_1  
#define DETECT_ZERO_PIN  GPIO_NUM_6 // chua co

#define BUTTON_PIN1      GPIO_NUM_8
#define BUTTON_PIN2      GPIO_NUM_3
#define BUTTON_PIN3      GPIO_NUM_7
#define BUTTON_PIN4      GPIO_NUM_0

#define LED_DATA         GPIO_NUM_21   
#define LED_CLK          GPIO_NUM_20   

#define	RELAY1_PIN	     GPIO_NUM_10		
#define	RELAY2_PIN	     GPIO_NUM_4
#define	RELAY3_PIN	     GPIO_NUM_5		
#define	RELAY4_PIN	     GPIO_NUM_6

#define TOUCH_ACTIVE_POW			0
#define TOUCH_NON_ACTIVE_POW		1

#define TOUCH                       1
#define NO_TOUCH                    0


void rd_gpio_init(void);
uint8_t rd_read_btn(gpio_num_t pin_btn);