#include <Arduino.h>

uint64_t blinkTimer = 0;
uint64_t scanTimer = 0;
int CSPin = 10;
char mode = '?';

void blinking(bool on);