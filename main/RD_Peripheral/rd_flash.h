#ifndef RD_FLASH_H
#define RD_FLASH_H

#include "esp_flash.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs.h"
#include "nvs_flash.h"

#define NAME_SPACE "RDstorage"

#define KEY_FLASH_CONFIG "RDConfig"
#define KEY_FLASH_SCENE  "RDScene"
#define KEY_FLASH_GROUP  "RDGroupTest"



// void rd_flash_read(uint32_t address, void *buffer, uint32_t length);
// void rd_flash_write(uint32_t address, void *buffer, uint32_t length);

void rd_write_flash(char* key, void *data, size_t length);
void rd_read_flash(char* key, void *data_rec, size_t length);

void rd_flash_init(void);
void rd_init_test_flash(void);

#endif /* RD_FLASH_H */
