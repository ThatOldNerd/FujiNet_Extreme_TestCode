#include <stdio.h>
#include <stdlib.h>
#include "esp_system.h"
#include "FijiNetConfig.h"
#include "RIOConfig.h"
#include "ESPHALDriverESP32-Wrover.h"

// Pull Z80 Reset Low

void Fill_RIOConfig_Data(){
int IOAddressTotal = 0;
// uint8_t TMPControl = 0x2;
uint8_t TMPData = 0x2;
Z80Data RIOData[256];
Z80Data RIOConfig[16];
//printf("*****Fill Data*****\n");
// Temporary variables to hold data to be stored into RIOConfig[]
uint16_t TMPAddress[16] = {
    0x2,0x4,0x6,0x8,
    0x10,0x12,0x14,0x16,
    0x18,0x20,0x22,0x24,
    0x26,0x26,0x28,0x30    
    };


for (int Count=0; Count<16; Count++ ) {
RIOConfig[Count].address = TMPAddress[Count];
RIOConfig[Count].controlMask = Test_WR;
RIOConfig[Count].data = TMPData;
} 
// move data to SPI Buss
    for (int Count=0; Count < 4 ; Count++ ){
        for (uint16_t IOAddress=0; IOAddress<=255;IOAddress++){
            for (int CheckByte=0; CheckByte<16; CheckByte++) {
            //printf("IOAddress %x RIOConfig %x \n",(int)IOAddress,(int)RIOConfig[CheckByte].address);
            if (IOAddress == RIOConfig[CheckByte].address){
                RIOData[IOAddress] = RIOConfig[IOAddress];
                //printf ("Match \n");
            }  else {
                RIOData[IOAddress].address = Count+IOAddressTotal;
                RIOData[IOAddress].controlMask = Test_WR;
                RIOData[IOAddress].data = 0x00;
                //printf("Fail \n");
            }
            }
            IOAddressTotal++;
            printf("IOTotal %x \n", IOAddressTotal);
        }
        WriteToRIO(RIOData,255);
    }
return;
}
