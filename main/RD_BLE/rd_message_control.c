#include "rd_message_control.h"
#include "rd_ble_mesh.h"

#define TAG "RD_MESS_CONTROL"

extern esp_ble_mesh_model_t vnd_models[];
extern esp_ble_mesh_model_t vnd_models1[];

static void RD_group_auto(uint16_t id_group, esp_ble_mesh_model_cb_param_t *param, uint16_t opcode_gr);
static void RD_handle_auto_createGR(esp_ble_mesh_model_cb_param_t *param);

void RD_Message_Control(esp_ble_mesh_model_cb_param_t *param) // E2
{
    uint8_t *buff_rsp;
    uint8_t *buf_rec = NULL;
    buf_rec = param->model_operation.msg;

    ESP_LOGI(TAG, "msg_size: %u, data_rec: ", param->model_operation.length);
    for (uint8_t i = 0; i < param->model_operation.length; i++)
    {
        printf("0x%02x ", buf_rec[i]);
    }
    printf("\n");

    uint16_t header = *(uint16_t *)param->model_operation.msg; // message
    ESP_LOGI(TAG, "Recv opcode 0x%06" PRIx32 ", header 0x%04x", param->model_operation.opcode, header);

    switch (header)
    {
    case RD_AUTO_CREATE_GS:
        RD_handle_auto_createGR(param);
        break;
    case RD_AUTO_CREATE_TEST: // test
        RD_group_auto(0xc000, param, ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE_ALL);
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

void rd_send_mess(esp_ble_mesh_model_cb_param_t *param, uint8_t *par, uint16_t addr_dst, uint16_t opcode)
{
    param->model_operation.ctx->addr = addr_dst;
    esp_err_t err = esp_ble_mesh_server_model_send_msg(&vnd_models[0],
                                                       param->model_operation.ctx, opcode,
                                                       sizeof(par), par);
    if (err)
    {
        ESP_LOGE(TAG, "Failed to send message 0x%06x", RD_OPCODE_RSP_FOR_E2);
    }
}

static void RD_handle_auto_createGR(esp_ble_mesh_model_cb_param_t *param)
{
    const uint16_t groupType = 0x001f;
    uint16_t id_group = 0;
    uint16_t id_group_type = 0;

    uint8_t *buff_rsp = param->model_operation.msg;
    rd_rsp_opcode_E2(param, buff_rsp);

    id_group = (buff_rsp[3] << 8) | buff_rsp[2];
    id_group_type = id_group + groupType;
    RD_group_auto(id_group, param, ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD);
    RD_group_auto(id_group_type, param, ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD);
}

static void RD_group_auto(uint16_t id_group, esp_ble_mesh_model_cb_param_t *param, uint16_t opcode_gr)
{
    uint16_t rd_primary_addr = esp_ble_mesh_get_primary_element_address();
    uint16_t model_id = ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_SRV;
    uint16_t company_id = 0xffff;      // SIG_MODEL: company_id = 0xffff

    if(opcode_gr == ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD){
        printf("add group: 0x%04x\n", id_group);
        esp_ble_mesh_model_subscribe_group_addr(rd_primary_addr, company_id, model_id, id_group);
    }else if(opcode_gr == ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE){
        printf("delete group: 0x%04x\n", id_group);
        esp_ble_mesh_model_unsubscribe_group_addr(rd_primary_addr, company_id, model_id, id_group);
    }else if(opcode_gr == ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE_ALL){
        printf("delete all group\n");
        uint8_t buf_send[4]= {0};
        buf_send[0] = rd_primary_addr & 0xff;
        buf_send[1] = (rd_primary_addr>>8) & 0xff;
        buf_send[2] = 0x00;
        buf_send[3] = 0x01; //sig model onoff 0x1000
        rd_send_mess(param, buf_send, rd_primary_addr, ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE_ALL);
    }
}
