#include <Arduino.h>
#include "ACMeasurement.h"
#include "ValueHistory.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Fonts/FreeMono12pt7b.h>

const int PIN_MEASURE_ACS712 = PIN_A1;
const double VOLTAGE_AC712 = 5.0;
const double MEASURED_VOLTAGE = 230.0;

Adafruit_SSD1306 display(128, 32, &Wire, -1);

ACMeasurement lastMeasurement;
ValueHistory smoothing(5);
ValueHistory histogram(128 - 24);
const int HISTOGRAM_INTERVAL = 15000;
unsigned long nextHistogramMillis = 0;

void setup() {
    pinMode(PIN_MEASURE_ACS712, INPUT);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    display.clearDisplay();
    display.display();

    nextHistogramMillis = millis();
}

double getRMSVoltage(double voltagePeak) {
    return (voltagePeak / 2.0) * 0.707;
}

double convertACS712VoltageToWatts(double voltage) {
    const double mvPerAmp = 0.1; // 100 mV per A for 20A sensor, see data sheet
    return voltage / mvPerAmp * MEASURED_VOLTAGE;
}

void showCurrentAvg() {
    display.setFont(&FreeMono12pt7b);

    auto average = round(smoothing.average());

    int printLen = 6;
    if (average < 10000) {
        printLen = 5;
    }
    if (average < 1000) {
        printLen = 4;
    }
    if (average < 1000) {
        printLen = 3;
    }
    if (average < 100) {
        printLen = 2;
    }
    if (average < 10) {
        printLen = 1;
    }

    display.setTextColor(WHITE);
    display.setCursor(64 - 10 * printLen, 22);
    display.print(average);
    display.println(" W");
}

void drawHistogram() {
    display.setFont();
    display.setTextSize(1);
    display.setCursor(0, 0);
    Serial.println("----------------");

    double histogramMax = max(0.0, histogram.maximum());
    unsigned int max = static_cast<unsigned int>(round(histogramMax));

    // Draw legend
    if (max >= 1000) {
        display.print(round(max / 1000.0));
        display.print("k");
    } else {
        display.print(max);
    }

    // Draw axis
    display.drawLine(23, 0, 23, 31, WHITE);
    display.drawLine(23, 31, 128, 31, WHITE);

    // Draw points
    for (int i = 0; i < histogram.size(); i++) {
        auto pointY = 30 - round(histogram[i] / histogramMax * 30);
        display.drawPixel(24 + i, static_cast<int16_t>(pointY), WHITE);
    }
}

void loop() {
    lastMeasurement.update(PIN_MEASURE_ACS712, 2000);

    double peakToPeakVoltage = max(0.0, lastMeasurement.getPeakToPeakVoltage(VOLTAGE_AC712));
    double rmsVoltage = max(0.0, getRMSVoltage(peakToPeakVoltage) - 0.01);
    double watts = convertACS712VoltageToWatts(rmsVoltage);

    smoothing.add(watts);
    display.clearDisplay();

    if (nextHistogramMillis <= millis()) {
        histogram.add(smoothing.average());

        if (histogram.size() > 1) {
            drawHistogram();
        } else {
            showCurrentAvg();
        }

        nextHistogramMillis = millis() + HISTOGRAM_INTERVAL;
    }
    else {
        showCurrentAvg();
    }

    display.display();

    delay(1000);
}
