#pragma once
struct Conditions {
    float temperature;
    float humidity;
    Conditions(float t, float h) : temperature(t), humidity(h) {};
    Conditions() : temperature(0), humidity(0) {};
};