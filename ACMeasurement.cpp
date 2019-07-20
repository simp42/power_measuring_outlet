#include "ACMeasurement.h"
#include <Arduino.h>

void ACMeasurement::update(uint8_t pin, unsigned long sampleTime) {
    int min = 1024;
    int max = 0;

    uint32_t start_time = millis();
    while ((millis() - start_time) < sampleTime) {
        auto measure = analogRead(pin);

        if (measure < min) {
            min = measure;
        }
        if (measure > max) {
            max = measure;
        }
    }

    this->min = min;
    this->max = max;
}

void ACMeasurement::update(uint8_t pin) {
    this->update(pin, 1000);
}

double ACMeasurement::getPeakToPeakVoltage(double inputVoltage) {
    double delta = static_cast<double>(this->max) - static_cast<double>(this->min);
    if (delta < 0) {
        return 0;
    }
    return delta * inputVoltage / 1024.0;
}
