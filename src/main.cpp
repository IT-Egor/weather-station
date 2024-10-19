#include <Arduino.h>

#include <AM2320.h>
#include <Wire.h>

AM2320 sensor;

void setup() {
	Serial.begin(115200);
	sensor.initialize(0x5C);
}

void loop() {
	Conditions cond = sensor.getConditions();
	Serial.print((String) "t="+cond.temperature+"°C, ");
	Serial.println((String) "h="+cond.humidity+"%");
	delay(1000);
}