#include <DataInterface.h>

DataInterface::DataInterface(int sensorAddres, int flashCSPin) : flash(flashCSPin) {
    sensor.initialize(sensorAddres);
    if (sensor.getStatus() == Status::INITIALIZED) {
        isSensorInitialized = true;
    } else {
        isSensorInitialized = false;
    }
    curFlashWriteAddress = 0;
    curFlashReadAddress = 0;
}

bool DataInterface::writeSensorDataToFlash() {
    Conditions conditions = sensor.getConditions();

    if (sensor.getStatus() == Status::OK) {
        uint8_t* data = convertConditionsToBytes(conditions);
        for (int i = 0; i < 4; i++) {
            flash.write(curFlashWriteAddress, data[i]);
            curFlashWriteAddress++;
        }

        Serial.println();
        Serial.println("write:");
        for (int i = 0; i < 4; i++) {
            Serial.print((String) data[i] + " ");
        }
        Serial.println();
        delete[] data;
        return true;
    } else {
        return false;
    }
}

Conditions DataInterface::readSensorDataFromFlash() {
    uint8_t data[4];
    for (int i = 0; i < 4; i++) {
        data[i] = flash.read(curFlashReadAddress);
        curFlashReadAddress++;
    }
    Serial.println();
    Serial.println("read:");
    for (int i = 0; i < 4; i++) {
        Serial.print((String) data[i] + " ");
    }
    Serial.println();
    return convertBytesToConditions(data);
}

uint8_t* DataInterface::convertConditionsToBytes(Conditions conditions) {

    uint16_t hum = static_cast<uint16_t>(conditions.humidity * 10);
    uint8_t humidityHighByte = (hum >> 8) & 0xFF;
    uint8_t humidityLowByte = hum & 0xFF;

    uint16_t temp;
    if (conditions.temperature >= 0) {
        temp = static_cast<uint16_t>(conditions.temperature * 10);
    } else {
        temp = static_cast<uint16_t>(-conditions.temperature * 10);
        temp |= 0x8000; // Установка бита знака
    }
    uint8_t temperatureHighByte = (temp >> 8) & 0xFF;
    uint8_t temperatureLowByte = temp & 0xFF;

    return new uint8_t[4] {humidityHighByte, humidityLowByte, temperatureHighByte, temperatureLowByte};
}

Conditions DataInterface::convertBytesToConditions(uint8_t data[4]) {
    Conditions conditions;
    uint16_t hum = ((data[0] << 8) | data[1]);
    conditions.humidity = hum / 10.0;

    uint16_t temp = (data[2] << 8 | data[3]);
    if ((data[2] & 0x80) >> 7 == 1) {
        conditions.temperature = -1 * temp / 10.0;
    } else {
        conditions.temperature = temp / 10.0;
    }

    return conditions;
}

bool DataInterface::sensorInitialized() {
    return isSensorInitialized;
}