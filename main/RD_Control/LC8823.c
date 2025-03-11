#include "LC8823.h"

extern void delay_us(uint32_t us);
led_switch_t led_switch[NUM_ELEMENT + 1];  // +1 led wifi

static void LC8823_send_bit(uint8_t bit_send){
    LED_CLK_LOW();
    delay_us(500000/FREQ_LED);
    LED_DATA_SET(bit_send); 
    LED_CLK_HIGH();
    delay_us(500000/FREQ_LED);
}

static void LC8823_send_byte(uint8_t byte_send){
    uint8_t bit_check;
    for(int8_t i = 7; i >= 0; i--){
        bit_check = (byte_send>>i) & 0x01;
        LC8823_send_bit(bit_check);
    }
}

static void start_frame_led(void){
    LC8823_send_byte(START_FRAME);
    LC8823_send_byte(START_FRAME);
    LC8823_send_byte(START_FRAME);
    LC8823_send_byte(START_FRAME);
}

static void end_frame_led(void){
    LC8823_send_byte(END_FRAME);
    LC8823_send_byte(END_FRAME);
    LC8823_send_byte(END_FRAME);
    LC8823_send_byte(END_FRAME);
    LED_CLK_HIGH();
}


void set_data_led(uint8_t lightness, uint8_t blue, uint8_t green, uint8_t red){
    LC8823_send_byte(lightness);
    LC8823_send_byte(blue);
    LC8823_send_byte(green);
    LC8823_send_byte(red);
}

// void LC8823_update_stt(void){
//     start_frame_led();

//     set_data_led(led_switch.LED_ARR[LED_1].lum, led_switch.LED_ARR[LED_1].blue, led_switch.LED_ARR[LED_1].green, led_switch.LED_ARR[LED_1].red);  // element 1
//     set_data_led(led_switch.LED_ARR[LED_1].lum, led_switch.LED_ARR[LED_1].blue, led_switch.LED_ARR[LED_1].green, led_switch.LED_ARR[LED_1].red); 

//     set_data_led(led_switch.LED_ARR[LED_2].lum, led_switch.LED_ARR[LED_2].blue, led_switch.LED_ARR[LED_2].green, led_switch.LED_ARR[LED_2].red);  // element 2
//     set_data_led(led_switch.LED_ARR[LED_2].lum, led_switch.LED_ARR[LED_2].blue, led_switch.LED_ARR[LED_2].green, led_switch.LED_ARR[LED_2].red); 
    
//     set_data_led(led_switch.LED_ARR[LED_3].lum, led_switch.LED_ARR[LED_3].blue, led_switch.LED_ARR[LED_3].green, led_switch.LED_ARR[LED_3].red);  // element 3
//     set_data_led(led_switch.LED_ARR[LED_3].lum, led_switch.LED_ARR[LED_3].blue, led_switch.LED_ARR[LED_3].green, led_switch.LED_ARR[LED_3].red); 
    
//     set_data_led(led_switch.LED_ARR[LED_4].lum, led_switch.LED_ARR[LED_4].blue, led_switch.LED_ARR[LED_4].green, led_switch.LED_ARR[LED_4].red);  // element 4
//     set_data_led(led_switch.LED_ARR[LED_4].lum, led_switch.LED_ARR[LED_4].blue, led_switch.LED_ARR[LED_4].green, led_switch.LED_ARR[LED_4].red); 
    
//     set_data_led(led_switch.LED_ARR[LED_WIFI].lum, led_switch.LED_ARR[LED_WIFI].blue, led_switch.LED_ARR[LED_WIFI].green, led_switch.LED_ARR[LED_WIFI].red);  // led wifi
    
//     end_frame_led();    
// }

void LC8823_update_stt(void){
    start_frame_led();

    set_data_led(led_switch[LED_1].led_data.lum, led_switch[LED_1].led_data.blue, led_switch[LED_1].led_data.green, led_switch[LED_1].led_data.red);  // element 1
    set_data_led(led_switch[LED_1].led_data.lum, led_switch[LED_1].led_data.blue, led_switch[LED_1].led_data.green, led_switch[LED_1].led_data.red); 

    set_data_led(led_switch[LED_2].led_data.lum, led_switch[LED_2].led_data.blue, led_switch[LED_2].led_data.green, led_switch[LED_2].led_data.red);  // element 2
    set_data_led(led_switch[LED_2].led_data.lum, led_switch[LED_2].led_data.blue, led_switch[LED_2].led_data.green, led_switch[LED_2].led_data.red); 
    
    set_data_led(led_switch[LED_3].led_data.lum, led_switch[LED_3].led_data.blue, led_switch[LED_3].led_data.green, led_switch[LED_3].led_data.red);  // element 3
    set_data_led(led_switch[LED_3].led_data.lum, led_switch[LED_3].led_data.blue, led_switch[LED_3].led_data.green, led_switch[LED_3].led_data.red); 
    
    set_data_led(led_switch[LED_4].led_data.lum, led_switch[LED_4].led_data.blue, led_switch[LED_4].led_data.green, led_switch[LED_4].led_data.red);  // element 4
    set_data_led(led_switch[LED_4].led_data.lum, led_switch[LED_4].led_data.blue, led_switch[LED_4].led_data.green, led_switch[LED_4].led_data.red); 
    
    set_data_led(led_switch[LED_WIFI].led_data.lum, led_switch[LED_WIFI].led_data.blue, led_switch[LED_WIFI].led_data.green, led_switch[LED_WIFI].led_data.red);  // led wifi
    
    end_frame_led();    
}

void led_set_stt(uint8_t num_led, uint8_t stt){
    led_switch[num_led].target = stt;
}

void led_set_control(uint8_t num_led){
    if(led_switch[num_led].current != led_switch[num_led].target){
        uint8_t stt = led_switch[num_led].target;
        led_switch[num_led].current = led_switch[num_led].target;
        led_switch[num_led].led_data.lum = stt ? LIGHT_MAX : LIGHT_MIN;
        LC8823_update_stt();
    }
}

void led_init(void){
    for(uint8_t i = 0; i< NUM_ELEMENT; i++){
        led_switch[i].current = 1;
        led_switch[i].target = 1;
        led_switch[i].led_data.lum = LIGHT_MAX;
        led_switch[i].led_data.blue = EN_COLOR;
        led_switch[i].led_data.green = DIS_COLOR;
        led_switch[i].led_data.red = DIS_COLOR;
    }

    led_switch[LED_WIFI].current = 1;
    led_switch[LED_WIFI].target = 1;
    led_switch[LED_WIFI].led_data.lum = LIGHT_MAX;
    led_switch[LED_WIFI].led_data.blue = DIS_COLOR;
    led_switch[LED_WIFI].led_data.green = DIS_COLOR;
    led_switch[LED_WIFI].led_data.red = EN_COLOR;
    LC8823_update_stt();
}










