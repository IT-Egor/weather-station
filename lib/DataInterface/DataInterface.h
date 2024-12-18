#include <Arduino.h>
#include <Conditions.h>
#include <AM2320.h>
#include <Flash.h>

class DataInterface {
public:
    DataInterface();
    bool writeSensorDataToFlash();
    Conditions readSensorDataFromFlash();
    bool initialize(int sensorAddres, int flashCSPin);
private:
    AM2320 sensor;
    Flash flash;
    bool isSensorInitialized;
    int curFlashWriteAddress;
    int curFlashReadAddress;

    uint8_t* convertConditionsToBytes(Conditions conditions);
    Conditions convertBytesToConditions(uint8_t bytes[4]);
};