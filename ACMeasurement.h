#ifndef LIB_ACMEASUREMENT_H
#define LIB_ACMEASUREMENT_H

#include <stdint-gcc.h>

class ACMeasurement {
public:
    int min;
    int max;

    void update(uint8_t pin);
    void update(uint8_t pin, unsigned long sampleTime);
    double getPeakToPeakVoltage(double inputVoltage);
};


#endif //LIB_ACMEASUREMENT_H
