#include "Arduino.h"
#include "AM2320.h"
#include "Wire.h"

AM2320::AM2320() {}

bool AM2320::initialize(uint8_t i2cAddress) {
    Wire.begin();

    Wire.beginTransmission(i2cAddress);
    Wire.endTransmission();

    Wire.beginTransmission(i2cAddress);
    uint8_t status = Wire.endTransmission();
    if (status == 0) {
        this->i2cAddress = i2cAddress;
        this->status = Status::INITIALIZED;
        return true;
    } else {
        this->status = Status::INITIALIZE_FAILED;
        return false;
    }
}

void AM2320::wakeUp() {
    Wire.beginTransmission(i2cAddress);
    Wire.endTransmission();
}

Conditions AM2320::getConditions() {
    if ( !processSensorResponse()) {
        return Conditions(-1, -1);
    }
    return conditions;
}

bool AM2320::readSensor() {
    wakeUp();

    uint8_t startAddress = 0x00;
    uint8_t writeAddress = 0x03;
    uint8_t writeBytes = 4;
    Wire.beginTransmission(i2cAddress);
    Wire.write(writeAddress);
    Wire.write(startAddress);
    Wire.write(writeBytes); 

    if (Wire.endTransmission(true) != 0) { 
        status = Status::SENSOR_NOT_READY;
        return false;
    }

    delayMicroseconds(1500); 

    uint8_t requestByteNumber = 8;
    Wire.requestFrom(i2cAddress, requestByteNumber); 

    for (int i = 0; i < requestByteNumber + 4; i++) {
        sensorResponse[i] = Wire.read();
    }

    return true;
}

bool AM2320::processSensorResponse() {
    if ( !readSensor()) {
        status = Status::READ_ERROR;
        return false;
    }

    uint16_t receivedCrc = ((sensorResponse[(int) ResponseBytes::CRC_HIGH_BYTE] << 8) 
        | sensorResponse[(int) ResponseBytes::CRC_LOW_BYTE]);

    uint8_t responseBytesNumber = 6;
    if (receivedCrc == crc16(sensorResponse, responseBytesNumber)) {

        uint16_t hum = (sensorResponse[(int) ResponseBytes::HUMIDITY_HIGH_BYTE] << 8) 
            | sensorResponse[(int) ResponseBytes::HUMIDITY_LOW_BYTE];
        conditions.humidity = hum / 10.0;

        uint16_t temp = (sensorResponse[(int) ResponseBytes::TEMPERATURE_HIGH_BYTE] << 8) 
            | sensorResponse[(int) ResponseBytes::TEMPERATURE_LOW_BYTE];

        if ((sensorResponse[(int) ResponseBytes::TEMPERATURE_HIGH_BYTE] & 0x80) >> 7 == 1) {
            conditions.temperature = -1 * temp / 10.0;
        } else {
            conditions.temperature = temp / 10.0;
        }
        status = Status::OK;
        return true;
    }
    else {
        status = Status::CRC_MISMATCH;
        return false;
    }
}

Status AM2320::getStatus() {
    return status;
}

unsigned short AM2320::crc16(unsigned char *ptr, unsigned char len) {
    unsigned short crc = 0xFFFF;
    while (len--) {
        crc ^= *ptr++;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x01) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}