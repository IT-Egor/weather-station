#include <Arduino.h>
#include <Conditions.h>
#include <AM2320.h>
#include <Flash.h>

class DataInterface {
public:
    DataInterface(int sensorAddres, int flashCSPin);
    bool writeSensorDataToFlash();
    Conditions readSensorDataFromFlash();
    bool sensorInitialized();
private:
    AM2320 sensor;
    Flash flash;
    bool isSensorInitialized;
    int curFlashWriteAddress;
    int curFlashReadAddress;

    uint8_t* convertConditionsToBytes(Conditions conditions);
    Conditions convertBytesToConditions(uint8_t bytes[4]);
};