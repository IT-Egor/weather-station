#include <Arduino.h>
#include <Definitions.h>
#include <AM2320.h>
#include <Wire.h>

AM2320 sensor;

void setup() {
	Serial.begin(115200);
	sensor.initialize(0x5C);
	if (sensor.getStatus() == Status::INITIALIZED) {
		Serial.println("AM2320 initialized");
	} else {
		Serial.println("AM2320 initialization failed");
	}
	Serial.println();
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
	Status sensorStatus;
	if (millis() - scanTimer > 1000) {
		scanTimer = millis();
		Conditions cond = sensor.getConditions();
		sensorStatus = sensor.getStatus();
		switch (sensorStatus) {
			case Status::OK:
				Serial.print((String) "t="+cond.temperature+"°C, ");
				Serial.println((String) "h="+cond.humidity+"%");
				break;
			
			case Status::READ_ERROR:
				Serial.println("AM2320 read error");
				break;

			case Status::SENSOR_NOT_READY:
				Serial.println("AM2320 sensor not ready");
				break;

			case Status::CRC_MISMATCH:
				Serial.println("AM2320 CRC mismatch");
				break;
		};
	}
	if (sensorStatus != Status::OK) {
		blinking(true);
	} else {
		blinking(false);
	}
}


void blinking(bool on) {
	if (on && millis() - blinkTimer > 100) {
		blinkTimer = millis();
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	}
}