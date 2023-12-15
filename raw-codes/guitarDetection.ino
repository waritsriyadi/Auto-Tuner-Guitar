#include "arduinoFFT.h"

#define SAMPLES 128
#define SAMPLING_FREQUENCY 2048

arduinoFFT FFT = arduinoFFT();

unsigned int samplingPeriod;
unsigned long microSeconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

void setup () {
  Serial.begin(115200);
  samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY));
}

void loop () {
  for (int i = 0; i < SAMPLES; i++) {
    microSeconds = micros();

    vReal[i] = analogRead(34);
    vImag[i] = 0;

    while (micros() < (microSeconds + samplingPeriod)){
      delay(1);
    }
  }
  // Perform FFT on samples
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  // Find peak frequency and print peak
  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

  if (peak > 675 && peak < 680) {
    Serial.print("e String detected:");
    Serial.println(peak);
  } else if (peak > 505 && peak < 511) {
    Serial.print("B String detected:");
    Serial.println(peak);
  } else if (peak > 400 && peak < 405) {
    Serial.print("G String detected:");
    Serial.println(peak);
  } else if (peak > 300 && peak < 305) {
    Serial.print("D String detected:");
    Serial.println(peak);
  } else if (peak > 450 && peak < 455) {
    Serial.print("A String detected:");
    Serial.println(peak);
  } else if (peak > 335 && peak < 340) {
    Serial.print("E String detected:");
    Serial.println(peak);
  }
  delay(100);
}