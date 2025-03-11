#include "rd_button.h"

extern gpio_num_t BUTTON_PIN_ARR[NUM_ELEMENT];
extern led_switch_t led_switch[NUM_ELEMENT + 1];

Button_Stt_Type_t button_Stt[NUM_ELEMENT] = {But_None};



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


