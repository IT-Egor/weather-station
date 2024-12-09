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



// #include <SPI.h>
// #include <Arduino.h>
// #include <Flash.h>

// const int CSPin = 10;

// byte arr[] = {0x18, 0x22, 0xA0, 0xAF, 0x20, 0xA8, 0xFF};
// Flash flash(10);

// void setup() {
//   Serial.begin(115200);
//   // pinMode(CSPin, OUTPUT);
//   // SPI.begin(); 
//   flash.sectorErase(0); delay(1000);// 0...255 sectors (1sector=4k_byte)
//  // block_clean(0);delay(2000);// 0...15 block (1block=64k_byte)
//  // chip_clean();delay(20000);
  
//   Serial.print("write: ");
//   for(int i=0; i<7; i++){
// 	Serial.print(arr[i]);
// 	Serial.print(" ");
//   }
//   Serial.println();

//   for(int i=0; i<7; i++) {
// 	flash.write(i+400, arr[i]);
//   }
// }

// void loop() { 
//   for(int i=0;i<7;i++) {
//   	Serial.print(flash.read(i+400), HEX);
// 	Serial.print(" ");
//   }
//   Serial.println();

//   delay(1000); 
// }

#include <Arduino.h>
#include <DataInterface.h>
#include <Definitions.h>

DataInterface di;
Conditions cond;

void setup() {
  Serial.begin(115200);
  Serial.println("start");

  while (mode == '?') {
    if (Serial.available()) {
      mode = Serial.read();
      Serial.println("ok");
      switch(mode) {
        case '1':
          Serial.println("read");
          di.initialize(0x5C, CSPin);
          break;
        case '0':
          Serial.println("write");
          break;
        default:
          Serial.print("Received unknown character: ");
          Serial.println(mode);
      }
    }
    // Serial.println("wait");
    // delay(1000);
  }

  // Flash flash;
  // flash.initialize(10);
  // // flash.chipErase();
  // di.initialize(0x5C, 10);
  // Serial.begin(115200);
  // Serial.println("start");
  // // Serial.println(di.initialize());

  // di.writeSensorDataToFlash();
  // Conditions cond = di.readSensorDataFromFlash();

  // Serial.println("test");
  // Serial.println(cond.humidity);
  // Serial.println(cond.temperature);
}

void loop() {
  // Serial.println("------------------------------------");
  // // di.writeSensorDataToFlash();
  // Conditions cond = di.readSensorDataFromFlash();
  // Serial.println("test");
  // Serial.println(cond.humidity);
  // Serial.println(cond.temperature);
  if (mode == '1') {
    Serial.println("begin");
    while (cond.temperature != 0.10 && cond.humidity != 6553.50) {
      cond = di.readSensorDataFromFlash();
      Serial.print((String) cond.temperature + ", ");
      Serial.println(cond.humidity);
      delay(10);
      if (cond.temperature == 0.10 && cond.humidity == 6553.50) {
        Serial.println("end");
      }
    }
    mode = '?';
  }
}