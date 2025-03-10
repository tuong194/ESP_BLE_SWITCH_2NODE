#include "rd_message_control.h"
#include "rd_ble_mesh.h"

extern rd_led_state led_state[NUM_ELEMENT];
#define TAG "RD_MESS_TYPE"

extern esp_ble_mesh_model_t vnd_models[];
extern esp_ble_mesh_model_t vnd_models1[];

RD_Type_Device_Message *RD_Mess_Temp_Receive;
RD_Type_Device_Message RD_Mess_Recevie;
RD_Save_GW_Address RD_Save_GW_Addr;
RD_Rsp_GW_Address RD_Rsp_GW_Addr;

uint16_t RD_GATEWAY_ADDR = 0x0001;
uint16_t saveGatewayAddr(uint8_t *para, uint16_t srcAddr);

void RD_Message_Type(esp_ble_mesh_model_cb_param_t *param)  // E0
{
    uint8_t *buff_rsp;
    uint8_t *buf_rec = NULL;
    buf_rec = param->model_operation.msg;
    RD_Mess_Temp_Receive = (RD_Type_Device_Message *)(&buf_rec[0]);
    RD_Mess_Recevie = *RD_Mess_Temp_Receive;
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
    case RD_HEADER_SET_TYPE_DEVICE:
        printf("set type\n");
        RD_Mess_Recevie.Header[0] = 0x01;
        RD_Mess_Recevie.Header[1] = 0x00;
        RD_Mess_Recevie.MainType = MAINTYPE;
        RD_Mess_Recevie.Feature = FEATURE;
        RD_Mess_Recevie.Name = NAME;
        RD_Mess_Recevie.Future[0] = 0x00;
        RD_Mess_Recevie.Future[1] = VERSION_MAIN;
        RD_Mess_Recevie.Future[2] = VERSION_SUB;

        buff_rsp = (uint8_t *)(&RD_Mess_Recevie.Header[0]);
        rd_rsp_opcode_E0(param, buff_rsp);
        break;
    case RD_HEADER_SAVE_GW_ADD:

        esp_ble_mesh_msg_ctx_t *rd_ctx = param->model_operation.ctx;
        RD_GATEWAY_ADDR = saveGatewayAddr(&param->model_operation.msg[2], rd_ctx->addr);
        printf("save GW addr: 0x%02x\n", rd_ctx->addr);
        RD_Flash_Save_GW(RD_GATEWAY_ADDR);
        RD_Save_GW_Addr.GWID[0] = (uint8_t)(RD_GATEWAY_ADDR & 0xff);
        RD_Save_GW_Addr.GWID[1] = (uint8_t)(RD_GATEWAY_ADDR >> 8 & 0xff);

        RD_Rsp_GW_Addr.Header[0] = 0x02;
        RD_Rsp_GW_Addr.Header[1] = 0x00;
        RD_Rsp_GW_Addr.GWID[0] = RD_Save_GW_Addr.GWID[0];
        RD_Rsp_GW_Addr.GWID[1] = RD_Save_GW_Addr.GWID[1];
        RD_Rsp_GW_Addr.Future[0] = PROVIDER_MAIN;
        RD_Rsp_GW_Addr.Future[1] = PROVIDER_SUB;
        RD_Rsp_GW_Addr.Future[2] = 0x00;
        RD_Rsp_GW_Addr.Future[3] = 0x00;
        break;
    case RD_GET_TYPE_AND_AES:
        printf("get type\n");
        RD_Mess_Recevie.Header[0] = 0x03;
        RD_Mess_Recevie.Header[1] = 0x00;
        RD_Mess_Recevie.MainType = MAINTYPE;
        RD_Mess_Recevie.Feature = FEATURE;
        RD_Mess_Recevie.Name = NAME;
        RD_Mess_Recevie.Future[0] = 0x00;
        RD_Mess_Recevie.Future[1] = VERSION_MAIN;
        RD_Mess_Recevie.Future[2] = VERSION_SUB;

        buff_rsp = (uint8_t *)(&RD_Mess_Recevie.Header[0]);
        rd_rsp_opcode_E0(param, buff_rsp);
        break;
    default:
        ESP_LOGI(TAG, "unknown header!!");
        break;
    }
}



void rd_rsp_opcode_E0(esp_ble_mesh_model_cb_param_t *param, uint8_t *par)
{
    esp_err_t err = esp_ble_mesh_server_model_send_msg(&vnd_models[0],
                                                       param->model_operation.ctx, RD_OPCDOE_TYPE_RSP,
                                                       sizeof(par), par);
    if (err)
    {
        ESP_LOGE(TAG, "Failed to send message 0x%06x", RD_OPCDOE_TYPE_RSP);
    }
}



uint16_t saveGatewayAddr(uint8_t *para, uint16_t srcAddr)
{
    uint16_t GW_Addr = 0x0001;
    if (para[0] || para[1])
    {
        GW_Addr = para[1] << 8 | para[0];
    }
    else
    {
        GW_Addr = srcAddr;
    }
    return GW_Addr;
}

static void example_change_led_state(esp_ble_mesh_model_t *model,
                                     esp_ble_mesh_msg_ctx_t *ctx, uint8_t onoff)
{
    uint16_t primary_addr = esp_ble_mesh_get_primary_element_address();
    uint8_t elem_count = esp_ble_mesh_get_element_count();
    uint8_t i;

    if (ESP_BLE_MESH_ADDR_IS_UNICAST(ctx->recv_dst))
    {
        for (i = 0; i < elem_count; i++)
        {
            if (ctx->recv_dst == (primary_addr + i))
            {
                rd_led_relay_operation(i, onoff);
            }
        }
    }
    else if (ESP_BLE_MESH_ADDR_IS_GROUP(ctx->recv_dst))
    {
        if (esp_ble_mesh_is_model_subscribed_to_group(model, ctx->recv_dst))
        {
            uint8_t num_ele = model->element->element_addr - primary_addr;
            printf("onoff group, element: %u\n", num_ele);
            rd_led_relay_operation(num_ele, onoff);
        }
    }
    else if (ctx->recv_dst == 0xFFFF)
    {

        for (i = 0; i < NUM_ELEMENT; i++)
        {
            rd_led_relay_operation(i, onoff);
        }
    }
}

void rd_mess_on_off_led(esp_ble_mesh_model_t *model, esp_ble_mesh_msg_ctx_t *ctx, uint8_t onoff)
{
    example_change_led_state(model, ctx, onoff);
}