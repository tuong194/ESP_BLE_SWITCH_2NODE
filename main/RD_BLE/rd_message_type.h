#ifndef C7D09179_9FDB_4800_9BE0_61E2CDB7924B
#define C7D09179_9FDB_4800_9BE0_61E2CDB7924B

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"
#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_local_data_operation_api.h"

#include "../RD_Control/rd_control_led_rl.h"
#include "rd_type_device.h"
#include "../RD_lib.h"

#define RD_HEADER_SET_TYPE_DEVICE		(0x0001)
#define RD_HEADER_SAVE_GW_ADD			(0x0002)
#define RD_GET_TYPE_AND_AES             (0x0003)

typedef struct{
	uint8_t Header[2];
	uint8_t MainType;
	uint8_t Feature;
	uint8_t Name;
	uint8_t Future[8];
}RD_Type_Device_Message;

typedef struct {
	uint8_t GWID[2];
}RD_Save_GW_Address;

typedef struct{
	uint8_t Header[2];
	uint8_t GWID[2];
	uint8_t Future[4];
}RD_Rsp_GW_Address;

void RD_Message_Type(esp_ble_mesh_model_cb_param_t *param);

void rd_rsp_opcode_E0(esp_ble_mesh_model_cb_param_t *param, uint8_t *par);
void rd_mess_on_off_led(esp_ble_mesh_model_t *model, esp_ble_mesh_msg_ctx_t *ctx, uint8_t onoff);


#endif /* C7D09179_9FDB_4800_9BE0_61E2CDB7924B */
