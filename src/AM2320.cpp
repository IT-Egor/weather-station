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
        Serial.println("AM2320: initialized");
        return true;
    } else {
        Serial.println("AM2320: initialize failed");
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
    uint8_t numByte = 4;
    Wire.beginTransmission(i2cAddress);
    Wire.write(0x03);
    Wire.write(startAddress);
    Wire.write(numByte); 

    if (Wire.endTransmission(true) != 0) { 
        Serial.println("AM2320: sensor not ready");
        return false;
    }
    delayMicroseconds(1500);                    
    Wire.requestFrom(i2cAddress, numByte + 4); 

    for (int i = 0; i < numByte + 4; i++) {
        sensorResponse[i] = Wire.read();
    }

    return true;
}

bool AM2320::processSensorResponse() {
    if ( !readSensor()) {
        Serial.println("AM2320: read error");
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

        return true;
    }
    else {
        Serial.println("AM2320: crc error");
        return false;
    }
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