#include <SPI.h>
#include <Flash.h>
#include <FlashUtility.h>

SPISettings spiSet(500000, MSBFIRST, SPI_MODE0);

Flash::Flash(int CSPin) {
    this->CSPin = CSPin;
    pinMode(CSPin, OUTPUT);
    SPI.begin(); 
}

byte Flash::read(long addr) {
    byte addr2 = (addr >> 16) & 0xFF;
    byte addr1 = (addr >> 8) & 0x00FF;
    byte addr0 = addr & 0x0000FF;
    
    SPI.beginTransaction(spiSet);
    digitalWrite(CSPin, LOW);
    SPI.transfer((int) Instructions::READ_DATA);
    SPI.transfer(addr2);
    SPI.transfer(addr1);
    SPI.transfer(addr0);

    byte data = SPI.transfer(0);
    
    digitalWrite(CSPin, HIGH);
    SPI.endTransaction();

    return data;
}

void Flash::write(long addr, byte data) {
    spiWriteEnable(true);

    byte addr2 = (addr >> 16) & 0xFF;
    byte addr1 = (addr >> 8) & 0x00FF;
    byte addr0 = addr & 0x0000FF;

    SPI.beginTransaction(spiSet);
    digitalWrite(CSPin, LOW);
    SPI.transfer((int) Instructions::PAGE_PROGRAM);
    SPI.transfer(addr2);
    SPI.transfer(addr1);
    SPI.transfer(addr0);

    SPI.transfer(data);

    digitalWrite(CSPin, HIGH);
    SPI.endTransaction();
    spiWriteEnable(false);
}

void Flash::sectorErase(byte addr) {
    spiWriteEnable(true);
    spiCommand((int) Instructions::SECTOR_ERASE, 0);
    spiWriteEnable(false);
}  

void Flash::blockErase(byte addr) {
    spiWriteEnable(true);
    spiCommand((int) Instructions::BLOCK_ERASE_32, 0);
    spiWriteEnable(false);
}   

void Flash::chipErase() {
    spiWriteEnable(true);
    spiWriteEnable((int) Instructions::CHIP_ERASE);
    spiWriteEnable(false);
} 

//-------------------------private--------------------------
void Flash::spiWriteEnable(bool enable) {
    SPI.beginTransaction(spiSet);
    digitalWrite(CSPin, LOW);
    if (enable) {
        SPI.transfer((int) Instructions::WRITE_ENABLE);
    } else {
        SPI.transfer((int) Instructions::WRITE_DISABLE);
    }
    digitalWrite(CSPin, HIGH);
    SPI.endTransaction();
}  

 void Flash::spiCommand(byte command, byte addr){
    SPI.beginTransaction(spiSet);
    digitalWrite(CSPin, LOW);
    SPI.transfer(command);
    SPI.transfer(0);
    SPI.transfer(0);
    SPI.transfer(addr);
    digitalWrite(CSPin, HIGH);
    SPI.endTransaction();
}       