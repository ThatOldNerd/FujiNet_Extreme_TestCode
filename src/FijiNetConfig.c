#include <stdio.h>
#include <stdlib.h>
#include "FijiNetConfig.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

void setESP32GPIOPin() {
    // Configure Z80 Conrtol Pins
    gpio_set_direction(Z80_Reset, GPIO_MODE_OUTPUT);
    gpio_set_direction(Z80_Hardlock, GPIO_MODE_INPUT);

    // Configure SPI Control Pins
    gpio_set_direction(ESP_Int, GPIO_MODE_INPUT);
    //gpio_set_direction(ESP_Wait_Reset, GPIO_MODE_INPUT);
    gpio_set_direction(ESP_RIO_Protect, GPIO_MODE_OUTPUT);
    gpio_set_direction(ESP_Hardlock, GPIO_MODE_INPUT);
    //gpio_set_direction(ESP_RomSel_0, GPIO_MODE_OUTPUT);
    gpio_set_direction(ESP_RomSel_1, GPIO_MODE_OUTPUT);

   // Configure Register Support Pins
    gpio_set_direction(ESP_SPI_INT_OE, GPIO_MODE_OUTPUT);
    gpio_set_direction(ESP_SPI_INT_STC, GPIO_MODE_OUTPUT);
    gpio_set_direction(ESP_SPI_INT_PL, GPIO_MODE_OUTPUT);
    gpio_set_direction(ESP_SPI_INT_CE, GPIO_MODE_OUTPUT);
}