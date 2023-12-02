#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "ESPHALDriverESP32-Wrover.h"
#include "FijiNetConfig.h"

spi_device_handle_t spi3;
int SPIReturn;

// *******************  Prepare SPI Bus for Data ***********************
// *********************************************************************

// initialize FujiNet SPI to send data
void spiInit() {
    esp_err_t ret;
    printf("Initialize SPI Buss \n");
    spi_bus_config_t buscfg={
        .miso_io_num = Fuji_MISO,
        .mosi_io_num = Fuji_MOSI,
        .sclk_io_num = Fuji_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32,
    };

    ret = spi_bus_initialize(SPI3_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    return;
}

    void spiTransmitConfig(){
    printf("Config SPI Buss \n");
    spi_device_interface_config_t devcfg={
        .command_bits = 0,
        .address_bits = 0,
        .clock_speed_hz = SPIFrequency,
        .mode = 0,                  //SPI mode 0
        .spics_io_num = Fuji_CS,     
        .queue_size = 1,
        .flags = SPI_DEVICE_HALFDUPLEX,
        //.flags = SPI_TRANS_USE_RXDATA,
        .pre_cb = NULL,
        .post_cb = NULL,
    };

    ESP_ERROR_CHECK(spi_bus_add_device(SPI3_HOST, &devcfg, &spi3));
    gpio_set_level(ESP_SPI_INT_CE, LOW);
    return;
};

// **********************Core SPI Functions****************************
//*********************************************************************

//Write Data to SPI
void WriteToSPI (Z80Data PacketData){
    printf("Transmit a Data Packet %x \n", PacketData.controlMask);
    spi_transaction_t t = {
        .flags = SPI_TRANS_USE_TXDATA,
        .length = 32,
        .tx_data[0] = (uint8_t)(PacketData.address >> 8),
        .tx_data[1] = (uint8_t)(PacketData.address),
        .tx_data[2] = PacketData.controlMask,
        .tx_data[3] = PacketData.data
    };
    ESP_ERROR_CHECK(spi_device_polling_transmit(spi3, &t)); // send the data
return;
}

//Clear Data to SPI
void ClearTheSPI (Z80Data PacketData){
    spi_transaction_t t = {
        .flags = SPI_TRANS_USE_TXDATA,
        .length = 32,
        .tx_data[0] = 0x00,
        .tx_data[1] = 0x00,
        .tx_data[2] = 0x00,
        .tx_data[3] = 0x00
    };
    ESP_ERROR_CHECK(spi_device_polling_transmit(spi3, &t)); // send the data
return;
}

// Read Data From SPI
Z80Data ReadFromSPI ()
{
    Z80Data ReadBuffer;
    ReadBuffer.address = 0;
    ReadBuffer.controlMask = 0;
    ReadBuffer.data = 0;
       spi_transaction_t t = {
        .flags = SPI_TRANS_USE_RXDATA,
        .length = 32,
        .rx_data[0] = (uint8_t)(ReadBuffer.address << 8),
        .rx_data[1] = (uint8_t)(ReadBuffer.address),
        .rx_data[2] = ReadBuffer.controlMask,
        .rx_data[3] = ReadBuffer.data
       };
    ESP_ERROR_CHECK(spi_device_polling_transmit(spi3, &t)); // send the data
return ReadBuffer;
}

// ********************************************************************
// ***************RIO Read and Write Routines**************************
// ********************************************************************
// Write multiple data points to FujiNet RIO

void WriteToRIO (Z80Data* PacketBuffer, int TotalAddress) 
{
  //gpio_set_level(ESP_Hardlock, HIGH); // Set the hardlock so the Z80 doesn't try to send
  for (int Count=0; Count<TotalAddress; Count++ )  // Continue sending until dataTotal is reached
  { 
    Z80Data TempData = PacketBuffer[Count];

    WriteToSPI(TempData);

    LoadWriteShiftRegister(); 

    printf("Send a Data Packet %x \n", TempData.controlMask);

    //ClearTheSPI(TempData);

    //LoadWriteShiftRegister();   
    }
    //gpio_set_level(ESP_Hardlock, LOW); // Clear the hardlock so the Z80 can access the board

return;
}

// Read multiple trasmissions from the RIO
Z80Data PacketBuffer[4096];
Z80Data * ReadFromRIO (int dataTotal)
{
//spi_device_acquire_bus(spi3, portMAX_DELAY);  // pull the buss low for multiple transactions
    //
    Z80Data TriggerRIO; // Temproary data to trigger address for the RIO
    for(int ReadCount = 0; ReadCount < dataTotal; ReadCount++){
    TriggerRIO.address = ReadCount;
    TriggerRIO.controlMask = CONTROLBYTE_RIOCONFIG_RD;
    TriggerRIO.data = 0xff;
    WriteToSPI(TriggerRIO);
    LoadWriteShiftRegister();
    LoadReadShiftRegister();

    PacketBuffer[ReadCount] = ReadFromSPI(); // Read data from the SPI Buss and place into the array
    
//spi_device_release_bus(spi3);  // release the buss
//SPIReturn = &PacketBuffer;
    }
return PacketBuffer;
}


//************************ Manipulate Data ************************************

// Combine assorted data to one variable ready to send or store
uint32_t combineData (Z80Data PacketBuffers)
{
  uint32_t DataCombined = ((uint32_t)PacketBuffers.address<<16)|((uint32_t)PacketBuffers.controlMask<<8)|PacketBuffers.data;

return DataCombined;
}

// Clear out the data packet
void clearBusPacketBuffer (Z80Data *PacketBuffer)
{
  PacketBuffer->data = 0xFF; // Data
  PacketBuffer->controlMask = 0xFF; // Control
  PacketBuffer->address = 0xFFFF; // Address
};

void LoadWriteShiftRegister(){
    //Trigger the HC595 Chips
    // LATCH the Shift Regs to Storage (pulse)
    gpio_set_level(ESP_SPI_INT_OE, HIGH);
    gpio_set_level(ESP_SPI_INT_STC, HIGH); // Move shift to storage
    //vTaskDelay(5 / portTICK_PERIOD_MS); //Delay for 5MS
    printf("Trigger \n");
    gpio_set_level(ESP_SPI_INT_STC, LOW);  
    gpio_set_level(ESP_SPI_INT_OE, LOW);  
    // LATCH data into the Shift Regs (pulse)

}
void LoadReadShiftRegister(){
    //Trigger the HC165 Chips
    gpio_set_level(ESP_SPI_INT_CE, HIGH);
    gpio_set_level(ESP_SPI_INT_PL, LOW);  // Parallel Load
    // Unload the Shift Reg to RXPacket
    gpio_set_level(ESP_SPI_INT_PL, HIGH); // Serial Shift
    gpio_set_level(ESP_SPI_INT_CE, LOW);
    // Load the data into the transaction
}