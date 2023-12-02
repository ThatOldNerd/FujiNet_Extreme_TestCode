#include "driver/spi_master.h"
#include "FijiNetConfig.h"


// Initialize SPI buss to send data half duplex
void spiInit();
// Prepare for half duplex transmission
void spiTransmitConfig();  

// ********************Basic SPI Comm********************
// Send Data to SPI Buss
void WriteToSPI (Z80Data PacketData);
// Clear Spi Control 
void ClearTheSPI (Z80Data PacketData);
// Read Data from SPI Buss
Z80Data ReadFromSPI ();

// ********************RIO Communictiaons****************
// Initialize SPI buss to recieve data half duplex
void WriteToRIO (Z80Data* PacketBuffer, int TotalAddress);

// Read multiple transactions from SPI Buss
Z80Data *ReadFromRIO (int dataTotal);

// ********************FujiNet Functions*********************
// Combine data struct into Unit32_t
uint32_t combineData (Z80Data PacketBuffers); 
// Clear Data Packet
void clearBusPacketBuffer (Z80Data *PacketBuffer);
// Routine to Trigger the HC595 Chips
void LoadWriteShiftRegister();
// Routine to Trigger the HC165 Chips  
void LoadReadShiftRegister();