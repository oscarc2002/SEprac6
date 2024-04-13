#ifndef MICROSD_H
#define MICROSD_H
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "UARTE.c"

#define EXAMPLE_MAX_CHAR_SIZE    64

static const char *TAG = "sd_card";

#define MOUNT_POINT "/sdcard"
#define PIN_NUM_MISO  19
#define PIN_NUM_MOSI  23
#define PIN_NUM_CLK   18
#define PIN_NUM_CS    5

const char mount_point[] = MOUNT_POINT;

typedef struct MicroSD_tag
{
    char Mount_Point[8];
    char Name_file[32];
    char Path[40];
    sdmmc_card_t *card;
} MicroSD_t;

static esp_err_t s_example_write_file(const char *path, char *data);

static esp_err_t s_example_read_file(const char *path);

static esp_err_t s_init_card(sdmmc_card_t *card);

#endif