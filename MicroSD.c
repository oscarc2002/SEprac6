#include "MicroSD.h"
const char *TAG = "sd_card";

const char *mount_point = MOUNT_POINT;

esp_err_t s_init_card(sdmmc_card_t *card)
{
    esp_err_t ret;
    /*  Options for mounting the filesystem.
        If format_if_mount_failed is set to true, SD card will be partitioned and
        formatted in case when mounting fails.*/
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    ESP_LOGI(TAG, "Inicializando SD card");

    /*  Use settings defined above to initialize SD card and mount FAT filesystem.
        Note: esp_vfs_fat_sdmmc/sdspi_mount is all-in-one convenience functions.
        Please check its source code and implement error recovery when developing
        production applications. */
    ESP_LOGI(TAG, "Usando periférico SPI");

    /*  By default, SD card frequency is initialized to SDMMC_FREQ_DEFAULT (20MHz)
        For setting a specific frequency, use host.max_freq_khz (range 400kHz - 20MHz for SDSPI)
        Example: for fixed frequency of 10MHz, use host.max_freq_khz = 10000;*/
    
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.max_freq_khz = 5000;
    host.slot = SPI3_HOST;

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    
    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Fallo inicializacion del bus.");
        return ESP_FAIL;
    }

    /*  This initializes the slot without card detect (CD) and write protect (WP) signals.
         Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals. */
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = host.slot;
    

    ESP_LOGI(TAG, "Montando el filesystem");
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Fallo montar el filesystem. "
                     "Si desea formatear la tarjeta, configure la opcion CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED.");
        } else {
            ESP_LOGE(TAG, "Fallo la inicializacion de la tarjeta (%s). "
                     "Asegurese de que las lineas de la tarjeta SD tengan resistencias pull-up en su lugar.", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Filesystem montado");

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);

    return ESP_OK;
}

esp_err_t s_example_write_file(const char *path, char *data)
{
    ESP_LOGI(TAG, "Abriendo archivo %s", path);
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        ESP_LOGE(TAG, "Fallo abrir el archivo para escritura");
        return ESP_FAIL;
    }
    fprintf(file, data);
    fclose(file);
    ESP_LOGI(TAG, "Archivo escrito");

    return ESP_OK;
}

esp_err_t s_example_read_file(const char *path)
{
    ESP_LOGI(TAG, "Abriendo archivo %s", path);
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        ESP_LOGE(TAG, "Fallo abrir el archivo para lectura");
        return ESP_FAIL;
    }
    char line[EXAMPLE_MAX_CHAR_SIZE];
    
    while(!feof(file))
    {
        fgets(line, sizeof(line), file);

        UART_puts(line);
    }
    //ESP_LOGI(TAG, "Lectura de archivo: '%s'", line);
    fclose(file);
    return ESP_OK;
}

void init_MicroSD(MicroSD_t *micro)
{
    memset(micro->Name_file, 0, sizeof(micro->Name_file));
    memset(micro->Path, 0, sizeof(micro->Path));
    strcpy(micro->Mount_Point, mount_point);
    ESP_LOGI(TAG, "init");
    s_init_card(micro->card);

}
