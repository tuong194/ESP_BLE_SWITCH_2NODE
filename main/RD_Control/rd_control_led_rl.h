#ifndef D49D0B9B_782B_4BF4_BCA4_72E8B770F9F9
#define D49D0B9B_782B_4BF4_BCA4_72E8B770F9F9

#include <stdio.h>
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_rom_sys.h"
#include "esp_timer.h"

#include "../RD_Peripheral/rd_gpio.h"

#define LED_ON      0
#define LED_OFF     1
#define RELAY_ON    1
#define RELAY_OFF   0

#define	NUM_CHECK_DETECH_MAX		150
#define TIME_DETECT_ON		6000	//us
#define TIME_DETECT_OFF		4800	//us

typedef struct {
    uint8_t current;
    uint8_t target;
}rd_led_state;

typedef struct {
    uint8_t current;
    uint8_t target;
}rd_relay_state;

typedef struct{
    uint8_t ele_cnt;
    uint8_t target;
}rd_queue_led_relay;

void rd_led_relay_operation(uint8_t element, uint8_t onoff);
void rd_led_relay_init(void);


#endif /* D49D0B9B_782B_4BF4_BCA4_72E8B770F9F9 */
