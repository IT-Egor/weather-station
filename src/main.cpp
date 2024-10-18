#include <Arduino.h>

void setup() {
	Serial.begin(115200);
	Serial.println("Blinking...");
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
	digitalWrite(LED_BUILTIN, HIGH);
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);
	delay(1000);
}