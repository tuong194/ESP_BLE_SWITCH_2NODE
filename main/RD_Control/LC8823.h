#pragma once

#include <stdio.h>
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../RD_Peripheral/rd_gpio.h"



#define FREQ_LED 25000

#define LED_DATA_SET(bit_send)   gpio_set_level(LED_DATA, bit_send)

#define LED_CLK_HIGH()    gpio_set_level(LED_CLK,1)
#define LED_CLK_LOW()     gpio_set_level(LED_CLK,0)

#define START_FRAME  0x00
#define END_FRAME    0xff
#define LIGHT_MAX    0xff  //100%
#define LIGHT_MIN    0xE2  //2/31 = 20%
#define EN_COLOR     0xff
#define DIS_COLOR    0x00
#define ON_STT       1
#define OFF_STT      0

typedef enum 
{
    LED_1 = 0,
    LED_4,
    LED_2,
    LED_3,
    LED_WIFI
}rd_led;

typedef struct{
    uint8_t lum;
    uint8_t blue;
    uint8_t green;
    uint8_t red;
}led_data_t;


typedef struct{
    uint8_t current;
    uint8_t target;
    led_data_t led_data; //+1 led wifi
}led_switch_t;

// typedef struct{
//     uint8_t current;
//     uint8_t target;
//     led_data_t LED_ARR[NUM_ELEMENT+1]; //+1 led wifi
// }led_switch_t;

void LC8823_update_stt(void);
void led_set_stt(uint8_t num_led, uint8_t stt);
void led_set_control(uint8_t num_led);
void led_init(void);
void blink_all_led(uint8_t num_blink);

