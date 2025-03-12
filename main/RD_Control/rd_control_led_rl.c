#include "rd_control_led_rl.h"

#define TAG "RD_LED_RL"

QueueHandle_t rd_queue;
static int64_t last_time_crl_relay = 0;

extern gpio_num_t RELAY_PIN_ARR[NUM_ELEMENT];

rd_relay_state relay_state[NUM_ELEMENT] = {
    { RELAY_OFF, RELAY_OFF },
    { RELAY_OFF, RELAY_OFF },
};

void delay_us(uint32_t us){
    esp_rom_delay_us(us);
}

uint8_t rd_exceed_us(int64_t ref, int64_t span_ms)
{
	return ((esp_timer_get_time() - ref) >= span_ms);
}

uint8_t read_detectzero_pin(void){
    uint8_t stt = 0;//gpio_get_level(DETECT_ZERO_PIN);
    return stt;
}

void rd_wait_detect_zero(void){
    uint8_t over_time_detect = 0;
    uint8_t stt_detect_past1, stt_detect_past2, stt_detect_cur1, stt_detect_cur2;
    stt_detect_past1 = stt_detect_past2 = stt_detect_cur1 = stt_detect_cur2 = read_detectzero_pin();
    do
    {
        over_time_detect ++;
        stt_detect_past1 = read_detectzero_pin();
        delay_us(50);
        stt_detect_past2 = read_detectzero_pin();
        delay_us(500);
        stt_detect_cur1 = read_detectzero_pin();
        delay_us(50);
        stt_detect_cur2 = read_detectzero_pin();

        if(over_time_detect >= NUM_CHECK_DETECH_MAX){
            printf("detect zero break\n");
            break;
        }
        
    } while (!(stt_detect_past1 != 0 && stt_detect_past2 != 0 && stt_detect_cur1 == 0 && stt_detect_cur2 == 0)); //falling
    last_time_crl_relay = esp_timer_get_time();
    
}
 

static void rd_set_relay_hw(gpio_num_t pin_relay, uint8_t stt){
    
    if(stt){
        gpio_set_level(pin_relay, RELAY_ON);
    }else{
        gpio_set_level(pin_relay, RELAY_OFF);
    }
}


void rd_led_relay_operation(uint8_t element, uint8_t onoff)
{
    relay_state[element].target = onoff;

    rd_queue_led_relay queue_send;
    queue_send.ele_cnt = element;
    queue_send.target = onoff;
    xQueueSend(rd_queue, &queue_send, portMAX_DELAY);
    //printf("queue co %d\n", uxQueueMessagesWaiting(rd_queue));
   
}

void update_control_led_relay(void){
    uint8_t element = 0, stt =0;
    rd_queue_led_relay queue_rec;
    
    if(xQueueReceive(rd_queue, &queue_rec, pdMS_TO_TICKS(1))){  //pdMS_TO_TICKS(1), portMAX_DELAY
        element = queue_rec.ele_cnt;
        stt = queue_rec.target;
        //printf("queue con %d phan tu\n", uxQueueMessagesWaiting(rd_queue));
        printf("onoff ele: %u\n", element);
        led_set_stt(element, stt);
        led_set_control(element);
    #if 0
        if(relay_state[element].current != relay_state[element].target){
            rd_wait_detect_zero(); 
            if(relay_state[element].target){
                if(rd_exceed_us(last_time_crl_relay, TIME_DETECT_ON)){
                    rd_set_relay_hw(RELAY_PIN_ARR[element], relay_state[element].target);
                    relay_state[element].current = relay_state[element].target;
                }
            }else{
                if(rd_exceed_us(last_time_crl_relay, TIME_DETECT_OFF)){
                    rd_set_relay_hw(RELAY_PIN_ARR[element], relay_state[element].target);
                    relay_state[element].current = relay_state[element].target;
                }
            }
        }
    #endif
    }
}


static void rd_led_relay_task(void *arg){
    rd_gpio_init();
    led_init();
    rd_queue = xQueueCreate(5, sizeof(rd_queue_led_relay)); //max 5 phan tu
    while (1)
    {
        //rd_scan_btn();
        rd_scan_btn_v2();
        check_time_kick_out();
        update_control_led_relay();
        vTaskDelay(10/portTICK_PERIOD_MS);

    }
}

void rd_led_relay_init(void)
{
    xTaskCreate(rd_led_relay_task, "rd_led_relay_task", 2048, NULL, 5, NULL);
}


 