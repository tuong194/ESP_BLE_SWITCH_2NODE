#ifndef C434178E_93F0_4189_904F_009EF2B68090
#define C434178E_93F0_4189_904F_009EF2B68090

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"
#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_local_data_operation_api.h"

#include "../RD_Control/rd_control_led_rl.h"
#include "rd_type_device.h"
#include "../RD_lib.h"

void RD_Message_Control(esp_ble_mesh_model_cb_param_t *param); 
void rd_rsp_opcode_E2(esp_ble_mesh_model_cb_param_t *param, uint8_t *par);

#endif /* C434178E_93F0_4189_904F_009EF2B68090 */
