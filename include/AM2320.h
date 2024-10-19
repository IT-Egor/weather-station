#include "Arduino.h"
#include "Conditions.h"

class AM2320 {
private:
    static const uint8_t maxResponseLength = 10;
    byte sensorResponse[maxResponseLength];
    Conditions conditions;
    uint8_t i2cAddress;

    bool readSensor();
    bool processSensorResponse();
    void wakeUp();
    unsigned short crc16(unsigned char *ptr, unsigned char len);

public:
    AM2320();
    bool initialize(uint8_t i2cAddress);
    Conditions getConditions();
    float getTemperature();

    enum class ResponseBytes : int {
        FUNCTION_CODE = 0,
        NUMBER_OF_BYTES = 1,
        HUMIDITY_HIGH_BYTE = 2,
        HUMIDITY_LOW_BYTE = 3,
        TEMPERATURE_HIGH_BYTE = 4,
        TEMPERATURE_LOW_BYTE = 5,
        CRC_LOW_BYTE = 6,
        CRC_HIGH_BYTE = 7,
    };
};
