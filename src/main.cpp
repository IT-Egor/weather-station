#include <Arduino.h>

#include <Wire.h> 
#include <iarduino_AM2320.h> 
iarduino_AM2320 sensor;

void setup() {                                                                    
	sensor.begin(&Wire);
	Serial.begin(115200);
}                                                                                
                                                                                 
void loop() {
	switch(sensor.read()) { 
		case AM2320_OK:
			Serial.print("Sensor AM2320: ");
			Serial.print((String) "t="+sensor.tem+"°C, ");
			Serial.print((String)"h="+sensor.hum+"%");
			Serial.println();
			break; 

		case AM2320_ERROR_LEN:
			Serial.println("Buffer size exceeded");
			break; 

		case AM2320_ERROR_ADDR: 
			Serial.println("The sensor is not detected");
			break; 

		case AM2320_ERROR_DATA: 
			Serial.println("Data error");
			break; 

		case AM2320_ERROR_SEND: 
			Serial.println("Transmission error");
		    break; 

		case AM2320_ERROR_READ: 
			Serial.println("Reading error");
		    break; 

		case AM2320_ERROR_ANS:  
			Serial.println("The answer is incorrect");
		    break; 

		case AM2320_ERROR_LINE: 
			Serial.println("Line interference"); 
			break; 
	}                                                                           
	delay(1000);                                                                
}                                                                                