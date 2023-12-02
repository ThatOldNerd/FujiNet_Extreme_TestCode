#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "sdkconfig.h"
//#include "esp_system.h"
#include "driver/gpio.h"
#include "FijiNetConfig.h"
#include "RIOConfig.h"
#include "ESPHALDriverESP32-Wrover.h"

uint8_t Z80_Locked;

void Data_Ready(void *pvParameter);

void app_main() {
    setESP32GPIOPin();
    spiInit();
    spiTransmitConfig();
    while(1){
    int test = gpio_get_level(Z80_Hardlock);
    if (test==0){
    printf("Z80 hardlock Set %x \n", test);}
    else {
    printf("Filling RIO \n");
    Fill_RIOConfig_Data();}
    vTaskDelay(15 / portTICK_PERIOD_MS); //Delay for 10MS
    }
    xTaskCreatePinnedToCore(&Data_Ready, "Data_Ready", 2048, NULL, 1, NULL, 1);
}

void Data_Ready(void *pvParameter){
// This task is meant to monitor the interrupt signal, pull and interperate the data

//   Z80Data Control_Byte;

    while(1){
        //if (gpio_get_level(Z80_Hardlock) == 1){
        //    Z80_Locked = 1;
        //} else {Z80_Locked = 0;}
    }
}