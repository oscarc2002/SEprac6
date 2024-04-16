#ifndef MICROSD_H
#define MICROSD_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#include "UARTE.h"

#define EXAMPLE_MAX_CHAR_SIZE    1024

extern const char *TAG;

#define MOUNT_POINT "/sdcard"
#define PIN_NUM_MISO  19
#define PIN_NUM_MOSI  23
#define PIN_NUM_CLK   18
#define PIN_NUM_CS    5
extern const char *TAG;
extern const char *mount_point;

typedef struct MicroSD_tag
{
    char Mount_Point[9];
    char Name_file[32];
    char Path[40];
    sdmmc_card_t *card;
} MicroSD_t;

esp_err_t s_example_write_file(const char *path, char *data);

esp_err_t s_example_read_file(const char *path);

esp_err_t s_init_card();

void init_MicroSD(MicroSD_t *micro);

#endif