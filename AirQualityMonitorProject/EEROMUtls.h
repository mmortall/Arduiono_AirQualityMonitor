//#define DEBUG_EEPROM
//#define DEBUG_TEST_EEPROM

#include <EEPROM.h>
#include <extEEPROM.h>
#define EEPROM_24LC512_address 0x50
extEEPROM eep(kbits_512, 1, 64); //one 24LC512 EEPROMS on the bus

#define FILE_VERSION 1
#define BLOCK_SIZE 8
#define HEADER_SIZE 12

uint32_t max_size = 0;
uint16_t max_blocks = 0;

void addDataBlock(uint16_t co2, uint16_t temp, uint16_t hum, uint16_t pres);
int getDataBlock(uint16_t blockPos, uint16_t& co2, uint16_t& temp, uint16_t& hum, uint16_t& pres);
void resetEEPROM();

uint16_t eep_read_uint16(uint32_t addr)
{
  uint8_t data[2];
  eep.read(addr, data, 2);
  uint16_t val = ((uint16_t)data[0] << 8) + (uint16_t)data[1];
  return val;  
}

void eep_write_uint16(uint32_t addr, uint16_t val)
{
#ifdef DEBUG_EEPROM
  Serial.print("eep_write_uint16: ");Serial.print(val);Serial.print(" to address ");Serial.println(addr);
#endif

  uint8_t data[2];
  data[0] = val >> 8;
  data[1] = val;  
  eep.write(addr, data, 2); 
}

uint8_t eep_read_uint8(uint8_t addr)
{
  uint8_t data[1];
  eep.read(addr, data, 1);
  uint8_t val = (uint8_t)data[0];
  return val;  
}

void eep_write_uint8(uint32_t addr, uint8_t val)
{
  uint8_t data[1];
  data[0] = val;
  eep.write(addr, data, 1); 
}

uint16_t eep_read_header_version()
{
  return eep_read_uint16(4);
}

uint16_t eep_read_header_size()
{
  return eep_read_uint16(6);
}

uint16_t eep_read_header_block_size()
{
  return eep_read_uint16(8);
}

uint16_t eep_read_header_num_of_blocks()
{
  return eep_read_uint16(10);
}

void eep_write_header_num_of_blocks(uint16_t val)
{
  eep_write_uint16(10, val);
}

uint16_t eep_read_header_last_block_adress()
{
  return eep_read_uint16(12);
}

void eep_write_header_last_block_adress(uint16_t val)
{
  eep_write_uint16(12, val);
}

void eep_write_clean_header(uint16_t vers, uint16_t header_size, uint16_t block_size, uint16_t blocks_num)
{
  eep_write_uint16(0, 12345);
  eep_write_uint16(2, 11111);
  eep_write_uint16(4, vers);
  eep_write_uint16(6, header_size);
  eep_write_uint16(8, block_size);
  eep_write_uint16(10, blocks_num);
  eep_write_uint16(12, 0 /*addr_of_first_block*/);
}

void eep_setup()
{
  delay(100);
#ifdef DEBUG
  Serial.print("eep_setup"); Serial.println();
#endif
  
  uint8_t eepStatus = eep.begin();
  
#ifdef DEBUG
  //Serial.print("eep_setup done"); Serial.println();
  Serial.print("EEPROM Inited with status:"); Serial.println(eepStatus);
#endif
  
  if (eepStatus != 0) {
    
#ifdef DEBUG_EEPROM    
  if(eepStatus == 1)
    Serial.print("Data too long to fit in transmit buffer"); Serial.println(eepStatus);
  if(eepStatus == 2)
    Serial.print("Received NACK on transmit of address"); Serial.println(eepStatus);     
#endif
  } 

  delay(100);

  //first read from eeprom is corrupted so do a couple of readings
  uint16_t hVer = eep_read_header_version();
  hVer = eep_read_header_version();
  hVer = eep_read_header_version();
  
  uint16_t hSize = eep_read_header_size();
  uint16_t hBlockSize = eep_read_header_block_size();
  uint16_t hNumOfBlocks = eep_read_header_num_of_blocks();
  uint16_t hLastBlockAddr = eep_read_header_last_block_adress();
  
  if(hVer != FILE_VERSION) //clean eeprom
  {
#ifdef DEBUG_EEPROM     
    Serial.print("extEEPROM had a swrong data format: "); 
    Serial.print("hVer: ");Serial.println(hVer);
    Serial.print("hSize: ");Serial.println(hSize);
    Serial.print("hBlockSize: ");Serial.println(hBlockSize);
    Serial.print("hNumOfBlocks: ");Serial.println(hNumOfBlocks);
    Serial.print("hLastBlockAddr: ");Serial.println(hLastBlockAddr);
    Serial.println(". Rewriting header");
#endif    
    resetEEPROM();
  }
  
  hVer = eep_read_header_version();
  
  if(hVer == 1)
  {
    hSize = eep_read_header_size();
    hBlockSize = eep_read_header_block_size();
    hNumOfBlocks = eep_read_header_num_of_blocks();
    hLastBlockAddr = eep_read_header_last_block_adress();

    max_size = 512*1024 - hSize;
    max_blocks = (uint16_t)floor((float)(max_size - hSize)/(float)hBlockSize);

#ifdef DEBUG_EEPROM    
    Serial.println("Reading extEEPROM:");
    Serial.print("hVer: ");Serial.println(hVer);
    Serial.print("hSize: ");Serial.println(hSize);
    Serial.print("hBlockSize: ");Serial.println(hBlockSize);
    Serial.print("hNumOfBlocks: ");Serial.println(hNumOfBlocks);
    Serial.print("hLastBlockAddr: ");Serial.println(hLastBlockAddr);
    Serial.print("max_size: ");Serial.println(max_size);
    Serial.print("max_blocks: ");Serial.println(max_blocks);
#endif    

#ifdef DEBUG_TEST_EEPROM    
    Serial.println("Begin EEPROM test:");
    resetEEPROM();
    uint16_t max_blocks_to_write = max_blocks;
    uint16_t additinal_orewlap = 20;
    max_blocks_to_write += additinal_orewlap;
    for(int i = 0; i < max_blocks_to_write; i++)
    {
      addDataBlock(i, 2*i, 123, 2*i);
    }

    bool isFailed = false;
    uint16_t numOfBlocks = eep_read_header_num_of_blocks();  
    for(uint16_t i = numOfBlocks-1; i >= 0; i--)
    {
      isFailed = true;
      uint16_t val_to_test = i + additinal_orewlap;
      uint16_t co2; uint16_t temp; uint16_t hum; uint16_t pres;
      uint16_t blockPos = i; 
      if(getDataBlock(blockPos, co2, temp, hum, pres) < 0)
      {
#ifdef DEBUG_EEPROM           
        Serial.print("getDataBlock error"); Serial.println(); break;
#endif          
      }
    

      //Serial.print("co2 data corrupted: "); Serial.print(co2); Serial.print(" but expected: ");Serial.println(val_to_test);
      //continue;
      
      if(co2 != val_to_test){
        Serial.print("co2 data corrupted: "); Serial.print(co2); Serial.print(" but expected: ");Serial.println(val_to_test); break;}
      if(temp != 2*val_to_test){
        Serial.print("temp data corrupted: "); Serial.println(temp); Serial.print(" but expected: ");Serial.println(2*val_to_test); break;}      
      if(hum != 123){
        Serial.print("hum data corrupted: "); Serial.println(hum); Serial.print(" but expected: ");Serial.println(123); break;}     
      if(pres != 2*val_to_test){
        Serial.print("pres data corrupted: "); Serial.println(pres); Serial.print(" but expected: ");Serial.println(2*val_to_test); break;} 

      isFailed = false;          

      if(i == 0)
         break;
    }
    if(isFailed)
      Serial.print("Test failed"); 
    else  
      Serial.print("Test success"); 
    Serial.println();
        
#endif    

    return;
  }
}

void resetEEPROM()
{
    eep_write_clean_header(FILE_VERSION, HEADER_SIZE, BLOCK_SIZE, 0);
}

void addDataBlock(uint16_t co2, uint16_t temp, uint16_t hum, uint16_t pres)
{
  uint16_t hSize = eep_read_header_size();  
  uint16_t blockSize = eep_read_header_block_size();
  uint16_t numOfBlocks = eep_read_header_num_of_blocks();  
  uint16_t fileOffset = eep_read_header_last_block_adress();

  uint32_t start_address = (uint32_t)hSize + ((uint32_t)fileOffset*(uint32_t)blockSize);
#ifdef DEBUG_EEPROM    
  Serial.print("Writing block "); Serial.print(fileOffset); Serial.print(" at address "); Serial.print(start_address); Serial.print(" with value: "); Serial.print(co2);
#endif  
  
  eep_write_uint16(start_address, co2); 
  uint16_t co2_verify = eep_read_uint16(start_address);
  start_address+=2;
  
  eep_write_uint16(start_address, temp); 
  uint16_t temp_verify = eep_read_uint16(start_address);
  start_address+=2;
  
  eep_write_uint16(start_address, hum); 
  uint16_t hum_verify = eep_read_uint16(start_address);
  start_address+=2;
  
#ifdef DEBUG_EEPROM 
  //Serial.print("co2_verify: "); Serial.print(co2_verify); Serial.print(" temp_verify: "); Serial.print(temp_verify);  
  //Serial.print(" hum_verify: "); Serial.print(hum); Serial.print(" == "); Serial.println(hum_verify);
   //Serial.println(); 
#endif 

  eep_write_uint16(start_address, pres); 
  start_address+=2;

  numOfBlocks += 1;
  if(numOfBlocks > max_blocks)
  {
    numOfBlocks -= 1;
  }
  else
  {
    eep_write_header_num_of_blocks(numOfBlocks);
  }

  fileOffset += 1;
  if(fileOffset == max_blocks)
  {
    fileOffset = 0;
  }  
    
  eep_write_header_last_block_adress(fileOffset);

#ifdef DEBUG_EEPROM    
  Serial.println(" DONE");
#endif    
}

int getDataBlock(uint16_t blockPos, uint16_t& co2, uint16_t& temp, uint16_t& hum, uint16_t& pres)
{
  uint16_t hSize = eep_read_header_size();  
  uint16_t blockSize = eep_read_header_block_size();
  uint16_t numOfBlocks = eep_read_header_num_of_blocks();  
  uint16_t fileOffset = eep_read_header_last_block_adress();

  if(blockPos >= numOfBlocks)  
  {
#ifdef DEBUG_EEPROM     
    Serial.print("ERROR: blockPos "); Serial.print(blockPos); Serial.print(" >= "); Serial.print(numOfBlocks);
#endif    
    return -1;
  }  

  uint32_t start_address = 0;
  uint16_t offsetToBlock = numOfBlocks - blockPos;
  if(offsetToBlock <= fileOffset)
  {
    start_address = (uint32_t)hSize + (((uint32_t)(fileOffset - offsetToBlock)) * (uint32_t)blockSize);      
  }
  else if(fileOffset < numOfBlocks)
  {
    start_address = (uint32_t)hSize + (((uint32_t)(fileOffset + blockPos)) * (uint32_t)blockSize);   
  }
  else
  {
    start_address = (uint32_t)hSize + (((uint32_t)blockPos) * (uint32_t)blockSize);   
  }
    
#ifdef DEBUG_EEPROM    
  Serial.print("Read block "); Serial.print(blockPos); Serial.print(" at address "); Serial.print(start_address);
#endif    

  co2 = eep_read_uint16(start_address); start_address+=2;
  temp = eep_read_uint16(start_address); start_address+=2;
  hum = eep_read_uint16(start_address); start_address+=2;  
  pres = eep_read_uint16(start_address); start_address+=2;

#ifdef DEBUG_EEPROM    
  Serial.println(" DONE");
#endif    

  return 0;  
}
