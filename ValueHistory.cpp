#include <HardwareSerial.h>
#include "ValueHistory.h"

ValueHistory::ValueHistory(const unsigned int length) {
    this->maxLength = length;
    this->currentLength = 0;
    this->history = new double[length];
}

ValueHistory::~ValueHistory() {
    delete[] this->history;
}

void ValueHistory::add(const double value) {
    while(this->currentLength >= this->maxLength) {
        this->shiftLeft();
    }

    this->history[this->currentLength] = value;
    this->currentLength++;
}

void ValueHistory::shiftLeft() {
    for(int i = 0; i < this->currentLength; i++) {
        this->history[i] = this->history[i + 1];
    }
    this->currentLength--;
}

double &ValueHistory::operator[](unsigned int index) {
    if (index > this->currentLength) {
        return this->history[0];
    }

    return this->history[index];
}

double ValueHistory::average() {
    double sum = 0.0;
    // TODO: do I care about possible overflow?
    for(int i = 0; i < this->currentLength; i++) {
        sum += this->history[i];
    }

    return sum / static_cast<double>(this->currentLength);
}

unsigned int ValueHistory::size() {
    return this->currentLength;
}

double ValueHistory::maximum() {
    double maximum = 0.0;

    for(int i = 0; i < this->currentLength; i++) {
        if (maximum < this->history[i]) {
            maximum = this->history[i];
        }
    }

    return maximum;
}
