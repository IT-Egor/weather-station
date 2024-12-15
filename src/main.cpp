#include <Arduino.h>
#include <Definitions.h>

void setup() {
    Serial.begin(115200);
    Serial.println("Choose mode:");
    Serial.println("r - read");
    Serial.println("w - write");
    Serial.println("p - print");
    Serial.print("mode = ");
    while (mode == '?') {
        if (Serial.available()) {
            mode = Serial.read();
            if (mode == 'r') {
                Serial.println("read");
            } else if (mode == 'w') {
                Serial.println("write");
                Flash flash;
                flash.initialize(CSPin);
                Serial.println("Memory erasing...");
                flash.chipErase();
                Serial.println("Memory erased");
            } else if (mode == 'p') {
                Serial.println("printing");
                sensor.initialize(sensorAddres);
            } else {
                Serial.print("unknown mode: ");
                Serial.println(mode);
                mode = '?';
                Serial.print("mode = ");
            }
        }
    }
    di.initialize(sensorAddres, CSPin);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    if (mode == 'r') {
        Serial.println("temperature, humidity");
        Serial.println("begin");
        do {
            cond = di.readSensorDataFromFlash();
            if (cond.humidity != 0 && cond.temperature != 0) {
                Serial.print((String) cond.temperature + ", ");
                Serial.println(cond.humidity);
            }
        } while (cond.temperature != 0 && cond.humidity != 0);
        Serial.println("end");
        mode = '?';

    } else if (mode == 'w') {
        while (scansCount < scansTotal) {
            if (millis() - scanTimer > timeBetweenScans) {
                scanTimer = millis();
                di.writeSensorDataToFlash();
                Serial.println("Measures: " + (String) scansCount + "/" + (String) scansTotal);
                blink();
                scansCount++;
            }
        }
        blinking(true);

    } else if (mode == 'p') {
        Status sensorStatus;
        if (millis() - scanTimer > timeBetweenScans) {
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
}

void blinking(bool on) {
	if (on && millis() - blinkingTimer > blinkingSpeed) {
		blinkingTimer = millis();
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	}
	if (!on) {
		digitalWrite(LED_BUILTIN, LOW);
	}
}

void blink() {
    if (millis() - lastBlinkTime > blinkSpeed) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        lastBlinkTime = millis();
    }
}