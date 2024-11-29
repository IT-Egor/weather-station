#include <SPI.h>

class Flash {
private:
    int CSPin;
    void spiWriteEnable(bool enable);
    void spiCommand(byte command, byte addr);
public:
    Flash(int CSPin);
    byte read(long addr);
    void write(long addr, byte data);
    void sectorErase(byte addr);
    void blockErase(byte addr);
    void chipErase();
};