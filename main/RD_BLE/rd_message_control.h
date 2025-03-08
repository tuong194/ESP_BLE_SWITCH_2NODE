#ifndef C7D09179_9FDB_4800_9BE0_61E2CDB7924B
#define C7D09179_9FDB_4800_9BE0_61E2CDB7924B

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"
#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_local_data_operation_api.h"

#include "../RD_Control/rd_control_led_rl.h"

void rd_mess_on_off_led(esp_ble_mesh_model_t *model, esp_ble_mesh_msg_ctx_t *ctx, uint8_t onoff);
void RD_Message_Control(esp_ble_mesh_model_cb_param_t *param);

#endif /* C7D09179_9FDB_4800_9BE0_61E2CDB7924B */
