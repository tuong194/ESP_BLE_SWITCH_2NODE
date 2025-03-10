#ifndef A7D1AC41_30EC_4981_82F0_16F83BFDEEF2
#define A7D1AC41_30EC_4981_82F0_16F83BFDEEF2

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"

#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_generic_model_api.h"
#include "esp_ble_mesh_local_data_operation_api.h"
#include "esp_ble_mesh_time_scene_model_api.h"
#include "ble_mesh_example_init.h"

#include "rd_message_control.h"
#include "rd_message_type.h"

#define CID_ESP                         0x0211
#define RD_OPCODE_TYPE_SEND            ESP_BLE_MESH_MODEL_OP_3(0xE0, CID_ESP)
#define RD_OPCDOE_TYPE_RSP             ESP_BLE_MESH_MODEL_OP_3(0xE1, CID_ESP)
#define RD_OPCODE_MESS_CONTROL         ESP_BLE_MESH_MODEL_OP_3(0xE2, CID_ESP)
#define RD_OPCODE_RSP_FOR_E2           ESP_BLE_MESH_MODEL_OP_3(0xE3, CID_ESP)

#define ESP_BLE_MESH_VND_MODEL_ID_CLIENT 0x0000
#define ESP_BLE_MESH_VND_MODEL_ID_SERVER 0x0001


void rd_init_ble(void);

#endif /* A7D1AC41_30EC_4981_82F0_16F83BFDEEF2 */
