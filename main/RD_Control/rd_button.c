#include "rd_button.h"
#include "esp_timer.h"

extern gpio_num_t BUTTON_PIN_ARR[NUM_ELEMENT];
extern led_switch_t led_switch[NUM_ELEMENT + 1];

Button_Stt_Type_t button_Stt[NUM_ELEMENT] = {But_None, But_None, But_None, But_None};
uint8_t button_check_keep_flag[NUM_ELEMENT] = {0};
int32_t time_check_kick_out = 0;

void rd_kick_out(void)
{
    printf("BUTTON reset: kick out\n");
    vTaskDelay(1000/portTICK_PERIOD_MS);
    blink_all_led(6);
    esp_ble_mesh_node_local_reset(); // resset mang
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    esp_restart();
}


extern uint8_t rd_exceed_us(int64_t ref, int64_t span_ms);


void rd_scan_btn(void){
    static uint8_t btn_count_buff[NUM_ELEMENT] = {0};
    static uint8_t stt = 0;

    for (uint8_t i = 0; i < NUM_ELEMENT; i++)
    {
        if(gpio_get_level(BUTTON_PIN_ARR[i]) == TOUCH){
            btn_count_buff[i]++;
            if (btn_count_buff[i] == 5)
            {
                button_Stt[i] = But_Press;
                printf("button %u press\n", i+1);
                stt = !(led_switch[i].current);
                led_set_stt(i, stt);
                led_set_control(i);
            }
             
        }else{
            button_Stt[i] = But_None;
            btn_count_buff[i] = 0;
        }
    }
}

static void switch_control_led(uint8_t num_ele){
    uint8_t stt = 0;
    stt = !(led_switch[num_ele].current);
    led_set_stt(num_ele, stt);
    led_set_control(num_ele);
}

void rd_scan_btn_v2(void){
    static uint8_t btn_count_buff[NUM_ELEMENT] = {0};
    static uint8_t btn_stt_temp[NUM_ELEMENT] = {0};


    for (uint8_t i = 0; i < NUM_ELEMENT; i++)
    {
        if(gpio_get_level(BUTTON_PIN_ARR[i]) == TOUCH){
            btn_count_buff[i]++;
            if (btn_count_buff[i] == COUNT_CYCLE_ACTIVE_BTN)
            {
                btn_stt_temp[i] = 1;
            }else if(btn_count_buff[i] > COUNT_CYCLE_ACTIVE_BTN){
                btn_stt_temp[i] += 1;
            }  
        }else{
            btn_stt_temp[i] = 0;
            btn_count_buff[i] = 0;
        }


        if( (btn_stt_temp[i] == 1) && (button_Stt[i] == But_None)){
            button_Stt[i] = But_Press;
            // printf("button %u press\n", i);
            // switch_control_led(i);
        }else if(btn_stt_temp[i]>=50 && button_Stt[i] == But_Press){
            printf("button %u is holding\n", i+1);
            time_check_kick_out = esp_timer_get_time();
            button_Stt[i] = But_Keeping;
            button_check_keep_flag[i] = 1;
        }
        else if(btn_stt_temp[i] == 0){
            if(button_Stt[i] == But_Keeping){
                printf("nha giu btn %u\n", i+1);
            }else if(button_Stt[i] == But_Press){
                printf("button %u press\n", i+1);
                switch_control_led(i);
            }
            button_Stt[i] = But_None;
            button_check_keep_flag[i] = 0;
        }
    }
}

void check_time_kick_out(void){
    static uint8_t count_kick_out = 0;
    if(button_check_keep_flag[0] == 1 && button_check_keep_flag[1] == 1){ 
        if(rd_exceed_us(time_check_kick_out, TIME_CHECK_KICK_OUT)){
            time_check_kick_out = esp_timer_get_time();
            count_kick_out++;
            printf("count number kick out: %u\n", count_kick_out);
            button_check_keep_flag[0] = 0;
            button_check_keep_flag[0] = 0;
            blink_all_led(2);
        }
    }
    if(count_kick_out == 3){
        count_kick_out = 0;
        rd_kick_out();
    }else if(count_kick_out < 3){
        if(rd_exceed_us(time_check_kick_out, TIME_OUT_KICK_OUT)){
            count_kick_out = 0;
        }
    }
}

