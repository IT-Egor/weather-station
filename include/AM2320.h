#include "Arduino.h"
#include "Conditions.h"
#include "Utility.h"

class AM2320 {
private:
    static const uint8_t maxResponseLength = 10;
    byte sensorResponse[maxResponseLength];
    Conditions conditions;
    uint8_t i2cAddress;
    Status status;

    bool readSensor();
    bool processSensorResponse();
    void wakeUp();
    unsigned short crc16(unsigned char *ptr, unsigned char len);

public:
    AM2320();
    bool initialize(uint8_t i2cAddress);
    Conditions getConditions();
    float getTemperature();
    Status getStatus();
};
