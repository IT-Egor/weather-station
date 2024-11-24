// #include <Arduino.h>
// #include <Definitions.h>
// #include <AM2320.h>
// #include <Wire.h>

// AM2320 sensor;

// void setup() {
// 	Serial.begin(115200);
// 	sensor.initialize(0x5C);
// 	if (sensor.getStatus() == Status::INITIALIZED) {
// 		Serial.println("AM2320 initialized");
// 	} else {
// 		Serial.println("AM2320 initialization failed");
// 	}
// 	Serial.println();
// 	pinMode(LED_BUILTIN, OUTPUT);
// }

// void loop() {
// 	Status sensorStatus;
// 	if (millis() - scanTimer > 1000) {
// 		scanTimer = millis();
// 		Conditions cond = sensor.getConditions();
// 		sensorStatus = sensor.getStatus();
// 		switch (sensorStatus) {
// 			case Status::OK:
// 				Serial.print((String) "t="+cond.temperature+"°C, ");
// 				Serial.println((String) "h="+cond.humidity+"%");
// 				break;
			
// 			case Status::READ_ERROR:
// 				Serial.println("AM2320 read error");
// 				break;

// 			case Status::SENSOR_NOT_READY:
// 				Serial.println("AM2320 sensor not ready");
// 				break;

// 			case Status::CRC_MISMATCH:
// 				Serial.println("AM2320 CRC mismatch");
// 				break;
// 		};
// 	}
// 	if (sensorStatus != Status::OK) {
// 		blinking(true);
// 	} else {
// 		blinking(false);
// 	}
// }


// void blinking(bool on) {
// 	if (on && millis() - blinkTimer > 100) {
// 		blinkTimer = millis();
// 		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
// 	}
// 	if (!on) {
// 		digitalWrite(LED_BUILTIN, LOW);
// 	}
// }

#include <SPI.h>
#include <Arduino.h>
#include <Flash.h>

const int SSPin = 10;

byte arr[] = {0x18, 0x22, 0xA0, 0xAF, 0x20, 0xA8, 0xFF};
Flash flash;

void setup() {
  Serial.begin(115200);
  pinMode(SSPin, OUTPUT);
  SPI.begin(); 
  flash.sectorErase(0); delay(1000);// 0...255 sectors (1sector=4k_byte)
 // block_clean(0);delay(2000);// 0...15 block (1block=64k_byte)
 // chip_clean();delay(20000);
  
  Serial.print("write: ");
  for(int i=0; i<7; i++){
	Serial.print(arr[i]);
	Serial.print(" ");
  }
  Serial.println();

  for(int i=0; i<7; i++) {
	flash.write(i+400, arr[i]);
  }
}

void loop() { 
  for(int i=0;i<7;i++) {
  	Serial.print(flash.read(i+400), HEX);
	Serial.print(" ");
  }
  Serial.println();

  delay(1000); 
}