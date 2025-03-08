#ifndef _MY_MQTT_H
#define _MY_MQTT_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "cJSON.h"

#define LED_BLUE  19

typedef void (*Mqtt_Callback_t)(esp_mqtt_event_handle_t);

void init_mqtt(void);
void rd_callback_sub(esp_mqtt_event_handle_t event);
void rd_register_cb_mqtt(Mqtt_Callback_t cb_func);
void rd_pub_mqtt(char* key, int value);

#endif /* _MY_MQTT_H */
