#include "rd_message_control.h"
#include "rd_ble_mesh.h"

#define TAG "RD_MESS_CONTROL"

extern esp_ble_mesh_model_t vnd_models[];
extern esp_ble_mesh_model_t vnd_models1[];

void RD_Message_Control(esp_ble_mesh_model_cb_param_t *param)  // E2
{
    uint8_t *buff_rsp;
    uint8_t *buf_rec = NULL;
    buf_rec = param->model_operation.msg;

    ESP_LOGI(TAG,"msg_size: %u, data_rec: ", param->model_operation.length);
    for (uint8_t i = 0; i < param->model_operation.length; i++)
    {
        printf("0x%02x ", buf_rec[i]);
    }
    printf("\n");

    uint16_t header = *(uint16_t *)param->model_operation.msg; // message
    ESP_LOGI(TAG, "Recv opcode 0x%06" PRIx32 ", header 0x%04x", param->model_operation.opcode, header);

    switch (header)
    {
    case 0x0001:

        break;
    default:
        ESP_LOGI(TAG, "unknown header!!");
        break;
    }
}

void rd_rsp_opcode_E2(esp_ble_mesh_model_cb_param_t *param, uint8_t *par)
{
    esp_err_t err = esp_ble_mesh_server_model_send_msg(&vnd_models[0],
                                                       param->model_operation.ctx, RD_OPCODE_RSP_FOR_E2,
                                                       sizeof(par), par);
    if (err)
    {
        ESP_LOGE(TAG, "Failed to send message 0x%06x", RD_OPCODE_RSP_FOR_E2);
    }
}