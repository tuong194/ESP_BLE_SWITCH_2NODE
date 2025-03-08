#ifndef _MY_GPIO_H
#define _MY_GPIO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"



#define ESP_INTR_FLAG_DEFAULT 0
#define IR_PIN 18
#define GPIO_INPUT_PIN_SEL ((1ULL << IR_PIN))

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO (5) // Define the output GPIO
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_10_BIT // Set duty resolution to 13 bits

#define LEDC_FREQUENCY (38000) // Frequency in Hertz. Set frequency at 4 kHz


void gpio_set_pin_input(gpio_num_t GPIO_NUM, gpio_int_type_t INTR_TYPE);
void gpio_set_pin_output(gpio_num_t GPIO_NUM);
void ledc_pwm_init(void);

#endif /* _MY_GPIO_H */
