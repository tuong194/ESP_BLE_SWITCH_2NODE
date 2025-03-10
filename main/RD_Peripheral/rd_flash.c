#include "rd_flash.h"

static const  char *FLASH_TAG = "FLASH_TAG";
static nvs_handle_t rd_nvs_handle;

/*---------------addr-----------------*/
void rd_flash_read(uint32_t address, void *buffer, uint32_t length){
    ESP_LOGI(FLASH_TAG,"Read flash at 0x%03x  \n", (unsigned int)address);
    ESP_ERROR_CHECK(esp_flash_read(NULL, buffer, address, length));
}

void rd_flash_write(uint32_t address, void *buffer, uint32_t length){
    ESP_LOGI(FLASH_TAG,"Write flash at 0x%03x  \n", (unsigned int)address);
    ESP_ERROR_CHECK(esp_flash_erase_region(NULL, address, 4096));
    ESP_ERROR_CHECK(esp_flash_write(NULL, buffer, address, length));
}


/*---------------key-value-----------------*/
void rd_write_flash(char* key, void *data, size_t length)
{
    esp_err_t err;
    err = nvs_open(NAME_SPACE, NVS_READWRITE, &rd_nvs_handle);
    if (err != ESP_OK) {
        printf("Failed to open NVS!\n");
        return;
    }
    //nvs_erase_key(rd_nvs_handle, key); // xoa du lieu trong key
    nvs_set_blob(rd_nvs_handle, key, (const void *)data, length);
    nvs_commit(rd_nvs_handle);
    nvs_close(rd_nvs_handle);
    printf("write data to %s\n", key);
}

void rd_read_flash(char* key, void *data_rec, size_t length)
{
    esp_err_t err;
    err= nvs_open(NAME_SPACE, NVS_READWRITE, &rd_nvs_handle);
    if (err != ESP_OK) {
        printf("Failed to open NVS!\n");
        return;
    }
    nvs_get_blob(rd_nvs_handle, key, (const void *)data_rec, &length);
    nvs_close(rd_nvs_handle);
    printf("read data from %s\n", key);

} 

void check_nvs_space() {
    nvs_stats_t nvs_stats;
    nvs_get_stats(NULL, &nvs_stats);
    printf("NVS Stats: Used entries = %d, Free entries = %d, Total entries = %d\n",
           nvs_stats.used_entries, nvs_stats.free_entries, nvs_stats.total_entries);
}

void rd_flash_init(void)
{
    esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
    check_nvs_space();
}






