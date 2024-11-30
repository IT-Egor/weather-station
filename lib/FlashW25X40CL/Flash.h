#include <SPI.h>

class Flash {
private:
    int CSPin;
    void spiWriteEnable(bool enable);
    void spiCommand(byte command, byte addr);
public:
    Flash();
    byte read(long addr);
    void write(long addr, byte data);
    void sectorErase(byte addr);
    void blockErase(byte addr);
    void chipErase();
    void initialize(int CSPin);
};