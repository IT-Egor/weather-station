#include <Arduino.h>
#include <DataInterface.h>

DataInterface di;
Conditions cond;
AM2320 sensor;

uint64_t blinkingTimer = 0;
unsigned int blinkingSpeed = 200;

uint64_t lastBlinkTime = 0;
uint64_t blinkSpeed = 1000;

uint64_t scanTimer = 0;
unsigned int timeBetweenScans = 1000;

long scansCount = 0;
long scansTotal = 86400;

byte sensorAddres = 0x5C;
int CSPin = 10;

char mode = '?';

void blinking(bool on);
void blink();