#include <SPI.h>
#include <Flash.h>
#include <FlashUtility.h>

const int SSPin = 10;

SPISettings spiSet(500000, MSBFIRST, SPI_MODE0);

Flash::Flash() {}

byte Flash::read(long addr) {
    byte addr2 = (addr >> 16) & 0xFF;
    byte addr1 = (addr >> 8) & 0x00FF;
    byte addr0 = addr & 0x0000FF;
    
    SPI.beginTransaction(spiSet);
    digitalWrite(SSPin, LOW);
    SPI.transfer((int) Instructions::READ_DATA);
    SPI.transfer(addr2);
    SPI.transfer(addr1);
    SPI.transfer(addr0);

    byte data = SPI.transfer(0);
    
    digitalWrite(SSPin, HIGH);
    SPI.endTransaction();

    return data;
}

void Flash::write(long addr, byte data) {
    spiWriteEnable(true);

    byte addr2 = (addr >> 16) & 0xFF;
    byte addr1 = (addr >> 8) & 0x00FF;
    byte addr0 = addr & 0x0000FF;

    SPI.beginTransaction(spiSet);
    digitalWrite(SSPin, LOW);
    SPI.transfer((int) Instructions::PAGE_PROGRAM);
    SPI.transfer(addr2);
    SPI.transfer(addr1);
    SPI.transfer(addr0);

    SPI.transfer(data);

    digitalWrite(SSPin, HIGH);
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

///////////////////////////////////////////////////////////////////////
void Flash::spiWriteEnable(bool enable) {
    SPI.beginTransaction(spiSet);
    digitalWrite(SSPin, LOW);
    if (enable) {
        SPI.transfer((int) Instructions::WRITE_ENABLE);
    } else {
        SPI.transfer((int) Instructions::WRITE_DISABLE);
    }
    digitalWrite(SSPin, HIGH);
    SPI.endTransaction();
}  

 void Flash::spiCommand(byte command, byte addr){
    SPI.beginTransaction(spiSet);
    digitalWrite(SSPin, LOW);
    SPI.transfer(command);
    SPI.transfer(0);
    SPI.transfer(0);
    SPI.transfer(addr);
    digitalWrite(SSPin, HIGH);
    SPI.endTransaction();
}       