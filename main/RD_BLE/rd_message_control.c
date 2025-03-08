#include "rd_message_control.h"


extern rd_led_state led_state[NUM_ELEMENT];
#define TAG "RD_MESS"

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
            uint8_t num_ele =  model->element->element_addr - primary_addr;
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


void rd_mess_on_off_led(esp_ble_mesh_model_t *model, esp_ble_mesh_msg_ctx_t *ctx, uint8_t onoff){
    example_change_led_state(model, ctx, onoff);
}

void RD_Message_Control(esp_ble_mesh_model_cb_param_t *param)
{
    uint8_t *buf_rec = NULL;
    buf_rec = param->model_operation.msg;
    printf("msg_size: %u, data_rec: ", param->model_operation.length);
    for (uint8_t i = 0; i < param->model_operation.length ; i++)
    {
        printf("0x%02x ", buf_rec[i]);
    }
    printf("\n");
    
}