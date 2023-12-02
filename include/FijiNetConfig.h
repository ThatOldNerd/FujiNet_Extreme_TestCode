#include "esp_system.h"

#ifndef _ESP32PinConfig_h
#define _ESP32PinConfig_h

/*
Z80 control
0 - RD -- Z80_RD / Local_RD
1 - WR -- Z80_WR / Local_WD
2 - IORQ -- Z80_IORQ / [Local] Cache_Data/Status (0/1)
3 - MEMRQ -- Z80_MEMRQ / Local_MEMRQ
4 - BUSRQ -- Z80_BUSRQ
5 - WAIT -- Z80_WAIT
6 - ROMCE -- Z80_ROMCE // HostROM ENABLED LOW, DISABLE HIGHn ( ZX spectrum )
7 - NMI -- Z80_NMI
                    7     6        5      4       3       2      1     0
// Control OUTPUT [ NMI | ROMCE  | WAIT | BUSRQ | MEMRQ | IORQ | WR  | RD ]
// Control INPUT  [ NMI | BUSACK | WAIT | BUSRQ | MEMRQ | IORQ | WR  | RD ]

*/ 

#define Test_WR             0b11101001
#define CONTROL_MASK_RD     0b00000001
#define CONTROL_MASK_WR     0b00000010
#define CONTROL_MASK_IORQ   0b00000100
#define CONTROL_MASK_MEMRQ  0b00001000

#define CONTROL_MASK_BUSRQ  0b00010000
#define CONTROL_MASK_WAIT   0b00100000
#define CONTROL_MASK_BUSACK 0b01000000
#define CONTROL_MASK_ROMCE  0b01000000
#define CONTROL_MASK_NMI    0b10000000


                                //    76543210
#define CONTROLBYTE_CLEAR           0b11111111

// Z80 NATIVE BUS OPERATIONS
// Memory access
#define CONTROLBYTE_MEMRQ_RD        0b11100110
#define CONTROLBYTE_MEMRQ_WR        0b11100101
// IO access
#define CONTROLBYTE_IORQ_RD         0b11101010
#define CONTROLBYTE_IORQ_WR         0b11101001

// Z80 FUJI OPERATIONS
// RIO-ROM (Z80 or ESP under BUSRQ)
#define CONTROLBYTE_CEROMRQ_RD      0b10100110
#define CONTROLBYTE_CEROMRQ_WR      0b10100101

// RIO-IODconfig (ESP under BUSRQ ONLY)
#define CONTROLBYTE_RIOCONFIG_RD    0b11101110
#define CONTROLBYTE_RIOCONFIG_WR    0b11101101


// CONTROL BYTE MASKS
#define CONTROLBYTE_BUSRQ_ENABLE    0b11101111
#define CONTROLBYTE_BUSRQ_DISABLE   0b11111111

#define CONTROLBYTE_HOSTROM_ENABLE  0b11111111
#define CONTROLBYTE_HOSTROM_DISABLE 0b10111111


// Local BUS
// CACHE data access
#define CONTROLBYTE_CACHEDATA_RD    0b10110010
#define CONTROLBYTE_CACHEDATA_WR    0b10110001
// CACHE status access
#define CONTROLBYTE_CACHESTATUS_RD  0b10110110
#define CONTROLBYTE_CACHESTATUS_WR  0b10110101


// Signals
//#define DISABLE_HIGH HIGH
//#define ENABLE_LOW LOW

//#define DISABLE_LOW LOW
//#define ENABLE_HIGH HIGH

//
// INTERUPT CODES

#define INT_NONE                0
#define INT_Z80HARDLOCK_RESET   1
#define INT_BUSACK              2
#define INT_WAIT                3

/*
    IOd CONFIG Byte

    0 - CACHE select 0 [1]
    1 - CACHE select 1 [1]
    2 - CACHE select 2 [1]
    3 - CACHE select 3 [1]
    4 - CACHE data[0]/status[1]
    5 - WAIT_IO [0]
    6 - RESET Z80HARDLOCK [1]
    7 - SET Z80HARDLOCK [1]
*/

                        // 76543210
#define IODCONFIG_STATUS 0b10010000 //(0x90) READ a status byte  -- Start IO 

// DATA IO address
#define IODCONFIG_CACHE0 0b10000000 //(0x80)
#define IODCONFIG_CACHE1 0b10000010 //(0x82)
#define IODCONFIG_CACHE2 0b10000100 //(0x84)
#define IODCONFIG_CACHE3 0b10001000 //(0x88)

#define IODCONFIG_IOEND  0b01010000 //(0x50) WRITE a status BYTE  -- End IO

// WAIT based addresses
#define IODCONFIG_WAIT   0b00100000 //(0x20)

// Z80 Control Pins
#define Z80_Reset GPIO_NUM_21
#define Z80_Hardlock GPIO_NUM_39

// ESP Control Pins
#define ESP_Int GPIO_NUM_36
#define ESP_Wait_Reset GPIO_NUM_11
#define ESP_RIO_Protect GPIO_NUM_4
#define ESP_Hardlock GPIO_NUM_2
#define ESP_RomSel_0 GPIO_NUM_7
#define ESP_RomSel_1 GPIO_NUM_6

// FujiNet SPI Pins
#define Fuji_MISO 19
#define Fuji_MOSI 23
#define Fuji_CLK  18
#define Fuji_CS   5 

// FujiNet SPI Support Pins
#define ESP_SPI_INT_OE GPIO_NUM_33   // OUTPUT (send/stop) --SPI Enable Output of an Address from shift registers
#define ESP_SPI_INT_STC GPIO_NUM_26  // D1  OUTPUT (pulse)     --SPI Latch Bus packet in shift registers for output
#define ESP_SPI_INT_PL  GPIO_NUM_25  // D2 OUTPUT (pulse)     -- SPI Load Host BUS state to shift registers
#define ESP_SPI_INT_CE  GPIO_NUM_17  // D3  -->>  CAN BE TIED TO GROUND
#define ESP_SPI_INT_SCK GPIO_NUM_18  // 

// External SPI Pins
#define PIN_External_MISO 12
#define PIN_External_MOSI 13
#define PIN_External_CLK  14
#define PIN_External_CS   15

// Universal Veriables
#define TickToWait 10  // 1 microsecond
#define SPIFrequency 5000000   // Speed of SPI Buss in Hz
#define HIGH 1 
#define LOW 0 

// Data Buss Structures

struct DataBuffer {
    uint8_t data;
    uint8_t controlMask;
    uint16_t address;
} ;

typedef struct DataBuffer Z80Data; 

// This function sets the GPIO pins' configuration
void setESP32GPIOPin();

#endif // _SET_PIN_CONFIG