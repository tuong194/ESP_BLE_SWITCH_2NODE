#include "rd_gpio.h"

gpio_num_t BUTTON_PIN_ARR[NUM_ELEMENT] = {BUTTON_PIN1, BUTTON_PIN3, BUTTON_PIN4, BUTTON_PIN2};
gpio_num_t RELAY_PIN_ARR[NUM_ELEMENT] = {RELAY1_PIN, RELAY2_PIN, RELAY3_PIN, RELAY4_PIN};

static void gpio_set_pin_input(gpio_num_t GPIO_NUM, gpio_int_type_t INTR_TYPE)
{
    gpio_config_t gpio_config_pin = {
        .pin_bit_mask = 1ULL << GPIO_NUM,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = INTR_TYPE
    };
    gpio_config(&gpio_config_pin);
    gpio_set_pull_mode(GPIO_NUM, GPIO_FLOATING);
    //gpio_evt_queue = xQueueCreate(30, sizeof(uint32_t));
}

static void gpio_set_pin_output(gpio_num_t GPIO_NUM){
     gpio_config_t gpio_config_pin = {
        .pin_bit_mask = 1ULL << GPIO_NUM,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
     };
     gpio_config(&gpio_config_pin);
}

void rd_gpio_init(void)
{
    for (uint8_t i = 0; i < NUM_ELEMENT; i++)
    {
        gpio_set_pin_input(BUTTON_PIN_ARR[i], GPIO_INTR_DISABLE);
        gpio_set_pin_output(RELAY_PIN_ARR[i]);
    }
    gpio_set_pin_output(LED_DATA);
    gpio_set_pin_output(LED_CLK);
    gpio_set_pin_output(RESET_TOUCH_PIN);
    //gpio_set_pin_input(DETECT_ZERO_PIN, GPIO_INTR_NEGEDGE);
    gpio_set_level(RESET_TOUCH_PIN, TOUCH_ACTIVE_POW);
    gpio_set_level(LED_CLK, 1);
}

