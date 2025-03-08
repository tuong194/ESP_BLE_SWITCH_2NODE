#ifndef _WIF_STA_H
#define _WIF_STA_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define TAG_WIFI "WIFI"

// #define ESP_WIFI_SSID "RD_SMART"
// #define ESP_WIFI_PASS "Led@RD@99" 
#define ESP_WIFI_SSID "DigitalR&D"
#define ESP_WIFI_PASS "DigitalRD@2804" 
#define MAX_RETRY_CONNECT 5
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

void init_wifi(void);

#endif /* _WIF_STA_H */
