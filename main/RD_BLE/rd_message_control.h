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
#include "rd_ble_mesh.h"

#define RD_AUTO_CREATE_GS					(0x0C0A)
#define RD_ON_OFF_ALL                       (0x000B)
#define RD_ASK_STT                          (0x090B)

#define RD_AUTO_CREATE_TEST					(0x0030)

void RD_Message_Control(esp_ble_mesh_model_cb_param_t *param); 
void rd_rsp_opcode_E2(esp_ble_mesh_model_cb_param_t *param, uint8_t *par, uint8_t length);
void rd_send_mess(esp_ble_mesh_model_cb_param_t *param, uint8_t *par, uint8_t length, uint16_t addr_dst, uint16_t opcode);

#endif /* C434178E_93F0_4189_904F_009EF2B68090 */
