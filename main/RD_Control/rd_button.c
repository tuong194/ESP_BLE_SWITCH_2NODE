#include "rd_button.h"

extern gpio_num_t BUTTON_PIN_ARR[NUM_ELEMENT];
extern led_switch_t led_switch[NUM_ELEMENT + 1];

Button_Stt_Type_t button_Stt[NUM_ELEMENT] = {But_None, But_None, But_None, But_None};
uint8_t button_check_keep_flag[NUM_ELEMENT] = {0};


void rd_scan_btn(void){
    static uint8_t btn_count_buff[NUM_ELEMENT] = {0};
    static uint8_t btn_stt[NUM_ELEMENT] = {0};
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
            printf("button %u holding\n", i);
            button_Stt[i] = But_Keeping;
            button_check_keep_flag[i] = 1;
        }
        else if(btn_stt_temp[i] == 0){
            if(button_Stt[i] == But_Keeping){
                printf("nha giu btn %u\n", i);
            }else if(button_Stt[i] == But_Press){
                printf("button %u press\n", i);
                switch_control_led(i);
            }
            button_Stt[i] = But_None;
            button_check_keep_flag[i] = 0;
        }
    }
}


