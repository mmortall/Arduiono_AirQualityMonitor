#include <EEPROM.h>
#include <extEEPROM.h>
#define EEPROM_24LC512_address 0x50
extEEPROM eep(kbits_512, 1, 64); //one 24LC512 EEPROMS on the bus

#define FILE_VERSION 1

uint32_t max_size = 0;

uint16_t eep_read_uint16(uint32_t addr)
{
  uint8_t data[2];
  eep.read(addr, data, 2);
  uint16_t val = ((uint16_t)data[0] << 8) + (uint16_t)data[1];
  return val;  
}

void eep_write_uint16(uint32_t addr, uint16_t val)
{
  uint8_t data[2];
  data[0] = val >> 8;
  data[1] = val;  
  eep.write(addr, data, 2); 
}

uint8_t eep_read_uint8(uint8_t addr)
{
  uint8_t val;
  eep.read(addr, val, 1);
  return val;  
}

void eep_write_uint8(uint32_t addr, uint8_t val)
{
  eep.write(addr, val, 1); 
}

uint16_t eep_read_header_version()
{
  return eep_read_uint16(0);
}

uint16_t eep_read_header_size()
{
  return eep_read_uint16(2);
}

uint16_t eep_read_header_block_size()
{
  return eep_read_uint16(4);
}

uint16_t eep_read_header_num_of_blocks()
{
  return eep_read_uint16(6);
}

uint16_t eep_read_header_last_block_adress()
{
  return eep_read_uint16(8);
}

void eep_write_clean_header(uint16_t vers, uint16_t header_size, uint16_t block_size, uint16_t blocks_num)
{
  eep_write_uint16(0, vers);
  eep_write_uint16(2, header_size);
  eep_write_uint16(4, block_size);
  eep_write_uint16(6, blocks_num);
  eep_write_uint16(8, 0 /*addr_of_first_block*/);
}

void eep_setup()
{
  uint8_t eepStatus = eep.begin();
  if (eepStatus != 0) {
  if(eepStatus == 1)
    Serial.print("Data too long to fit in transmit buffer"); Serial.println(eepStatus);
  if(eepStatus == 2)
    Serial.print("Received NACK on transmit of address"); Serial.println(eepStatus);     
  } 

  uint16_t hVer = eep_read_header_version();
  if(hVer != FILE_VERSION) //clean eeprom
  {
    Serial.println("extEEPROM had a swrong data format. Rewriting header");
    eep_write_clean_header(FILE_VERSION, 10, 7, 0);
  }
  
  hVer = eep_read_header_version();
  
  if(hVer == 1)
  {
    uint16_t hSize = eep_read_header_size();
    uint16_t hBlockSize = eep_read_header_block_size();
    uint16_t hNumOfBlocks = eep_read_header_num_of_blocks();
    uint16_t hLastBlockAddr = eep_read_header_last_block_adress();

    max_size = 512*1024 - hSize;

    Serial.println("Reading extEEPROM:");
    Serial.print("hSize: ");Serial.println(hSize);
    Serial.print("hBlockSize: ");Serial.println(hBlockSize);
    Serial.print("hNumOfBlocks: ");Serial.println(hNumOfBlocks);
    Serial.print("hLastBlockAddr: ");Serial.println(hLastBlockAddr);
    return;
  }
}

void addDataBlock(uint16_t co2, uint16_t temp, uint8_t hum, uint16_t pres)
{
  uint16_t hSize = eep_read_header_size();  
  uint16_t hBlockSize = eep_read_header_block_size();
  uint16_t hNumOfBlocks = eep_read_header_num_of_blocks();  
  uint16_t hFileOffset = eep_read_header_last_block_adress();
  
  //uint32_t addrOfFirstBlock = (uint32_t)hSize + (((uint32_t)hNumOfBlocks) * (uint32_t)hBlockSize);
  //if(addrOfFirstBlock > max_size)

  uint32_t start_address = hSize + hFileOffset*hBlockSize;
  eep_write_uint16(start_address, co2); start_address+=2;
  eep_write_uint16(start_address, temp); start_address+=2;
  eep_write_uint8(start_address, hum); start_address+=1;
  eep_write_uint16(start_address, pres); start_address+=2;
  
  eep_write_header_num_of_blocks(hNumOfBlocks + 1);
  eep_write_header_last_block_adress(hFileOffset + 1);
  if(start_address >= max_size - hBlockSize) //if we are not able to write more data by adding to the end
  {
    eep_write_header_last_block_adress(0);
  }
}
