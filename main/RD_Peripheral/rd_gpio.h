#pragma once

#include <stdio.h>
#include "driver/gpio.h"

/*18, 19: D+ D-
IO9: boot
20,21: tx rx*/

#define NUM_ELEMENT  2

#define RESET_TOUCH_PIN  GPIO_NUM_4  //
#define DETECT_ZERO_PIN  GPIO_NUM_5

#define BUTTON_PIN1  GPIO_NUM_2
#define BUTTON_PIN2  GPIO_NUM_3

#define LED_1        GPIO_NUM_6   // red
#define LED_2        GPIO_NUM_7   // green

#define	RELAY1_PIN	 GPIO_NUM_8			
#define	RELAY2_PIN	 GPIO_NUM_10

#define 	TOUCH_ACTIVE_POW			0 
#define 	TOUCH_NON_ACTIVE_POW		1


void rd_gpio_init(void);
uint8_t rd_read_btn(gpio_num_t pin_btn);