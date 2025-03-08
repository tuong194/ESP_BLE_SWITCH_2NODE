#include "my_mqtt.h"
#include "my_gpio.h"

#define TAG_MQTT "MQTT"

Mqtt_Callback_t mqtt_callback;
esp_mqtt_client_handle_t client;

static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0)
    {
        ESP_LOGE(TAG_MQTT, "Last error %s: 0x%x", message, error_code);
    }
}
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG_MQTT, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_CONNECTED");

        // msg_id = esp_mqtt_client_publish(client, "v1/devices/me/telemetry", "{\"temp\":50}", 0, 1, 0);
        // ESP_LOGI(TAG_MQTT, "sent publish successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "v1/devices/me/rpc/request/+", 0);
        ESP_LOGI(TAG_MQTT, "sent subscribe successful, msg_id=%d", msg_id);
        rd_register_cb_mqtt(rd_callback_sub);

        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        // msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        // ESP_LOGI(TAG_MQTT, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DATA");
        mqtt_callback(event); // call back func

        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno", event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG_MQTT, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        ESP_LOGI(TAG_MQTT, "Other event id:%d", event->event_id);
        break;
    }
}

static void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://demo.thingsboard.io",
        .broker.address.port = 1883,
        .credentials.username = "temdM5kIuV0rzeL9Wiri", // access tocken
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

void init_mqtt(void)
{
    // ESP_LOGI(TAG_MQTT, "[APP] Startup..");
    // ESP_LOGI(TAG_MQTT, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    // ESP_LOGI(TAG_MQTT, "[APP] IDF version: %s", esp_get_idf_version());

    // esp_log_level_set("*", ESP_LOG_INFO);
    // esp_log_level_set("mqtt_client", ESP_LOG_VERBOSE);
    // esp_log_level_set("mqtt_example", ESP_LOG_VERBOSE);
    // esp_log_level_set("transport_base", ESP_LOG_VERBOSE);
    // esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    // esp_log_level_set("transport", ESP_LOG_VERBOSE);
    // esp_log_level_set("outbox", ESP_LOG_VERBOSE);

    mqtt_app_start();
}

/* JSON */

void parse_json(const char *json_string)
{

    cJSON *json = cJSON_Parse(json_string);

    if (json == NULL)
    {
        printf("json NULL\n");
        return;
    }

    cJSON *id = cJSON_GetObjectItem(json, "id");
    if (cJSON_IsNumber(id))
    {
        printf("ID: %d\n", id->valueint);
    }

    cJSON *name = cJSON_GetObjectItem(json, "name");
    if (cJSON_IsString(name))
    {
        printf("Name: %s\n", name->valuestring);
    }

    cJSON *temperature = cJSON_GetObjectItem(json, "temperature");
    if (cJSON_IsNumber(temperature))
    {
        printf("Temperature: %.2f\n", temperature->valuedouble);
    }

    cJSON *status = cJSON_GetObjectItem(json, "status");
    if (cJSON_IsString(status))
    {
        printf("Status: %s\n", status->valuestring);
    }

    // release
    cJSON_Delete(json);
}

void test_json(void)
{
    const char *json_string = "{\"id\": 1, \"name\": \"ESP32\", \"temperature\": 36.5, \"status\": \"active\"}";
    parse_json(json_string);
}

void rd_callback_sub(esp_mqtt_event_handle_t event)
{
    printf("Topic=%.*s\r\n", event->topic_len, event->topic);
    printf("Data=%.*s\r\n", event->data_len, event->data);

    // char *rd_topic = (char*) malloc(event->topic_len+1);
    // strncpy(rd_topic, event->topic, event->topic_len);
    // rd_topic[event->topic_len] = '\0';
    // printf("data_rec: %.*s\r\n", event->data_len + 1, rd_topic);

    cJSON *json = cJSON_Parse(event->data);

    if (json == NULL)
    {
        printf("data NULL\n");
        return;
    }
    cJSON *params = cJSON_GetObjectItem(json, "params");
    if (cJSON_IsBool(params))
    {
        printf("status: %d\n", params->valueint);
        if (params->valueint)
        {
            //gpio_set_level(LED_G, 1);
        }
        else
        {
            //gpio_set_level(LED_G, 0);
        }
    }

    // free(rd_topic);
}
void rd_register_cb_mqtt(Mqtt_Callback_t cb_func)
{
    mqtt_callback = cb_func;
}

void rd_convert_json_to_string(char *key, char *value)
{
    char *string = NULL;
    cJSON *root = cJSON_CreateObject();
    if (cJSON_AddStringToObject(root, "method", "listdevzone") == NULL)
    {
        cJSON_Delete(root);
    }
    string = cJSON_Print(root);
    printf("%s ", string);
    free(string);
}

void rd_pub_mqtt(char* key, int value){
    char *string_send = NULL;
    cJSON *root = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(root, key, value) == NULL)
    {
        cJSON_Delete(root);
    }
    string_send = cJSON_Print(root);
    esp_mqtt_client_publish(client, "v1/devices/me/telemetry", string_send, 0, 1, 0);
    printf("public data: %s \n", string_send);
    free(string_send);
    
}

