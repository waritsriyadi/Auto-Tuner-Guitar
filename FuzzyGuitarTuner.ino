#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "arduinoFFT.h"
#include <Stepper.h>
#include <Fuzzy.h>

#define SAMPLES 128
#define SAMPLING_FREQUENCY 2048
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Define the number of steps per revolution
const int stepsPerRevolution = 256;
// Define the motor control pins
const int motorPin1 = 17;
const int motorPin2 = 5;
const int motorPin3 = 18;
const int motorPin4 = 19;
unsigned int samplingPeriod;
unsigned long microSeconds;
double vReal[SAMPLES];
double vImag[SAMPLES];

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
arduinoFFT FFT = arduinoFFT();
Fuzzy *fuzzy = new Fuzzy();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Stepper myStepper(stepsPerRevolution, motorPin1, motorPin3, motorPin2, motorPin4);

// Menu definition
const char *menuItems[] = { "Senar E", "Senar A", "Senar D", "Senar G", "Senar B", "Senar e" };
int selectedItem = 0;
int itemCount = sizeof(menuItems) / sizeof(menuItems[0]);
int firstVisibleItem = 0;  // Index of the first visible item
bool inSubMenu = false;

// Button pin definitions
#define CHANGE_BUTTON 13  // Change with the appropriate pin
#define OK_BUTTON 12      // Change with the appropriate pin
#define BACK_BUTTON 14    // Change with the appropriate pin

void setup() {
  Serial.begin(115200);
  samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
  myStepper.setSpeed(32);  // RPM

  pinMode(CHANGE_BUTTON, INPUT_PULLUP);
  pinMode(OK_BUTTON, INPUT_PULLUP);
  pinMode(BACK_BUTTON, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("=====================");
  display.println("=                   =");
  display.println("=                   =");
  display.println("=                   =");
  display.println("=                   =");
  display.println("=                   =");
  display.println("=                   =");
  display.setCursor(0, 59);
  display.print("=====================");
  display.setCursor(48, 23);
  display.print("Fuzzy");
  display.setCursor(26, 35);
  display.print("Guitar Tunner");
  display.display();

  //FUZZY INITIATION
  FuzzyInput *freqError = new FuzzyInput(1);
  FuzzySet *small = new FuzzySet(-50, -30, -20, 0);
  freqError->addFuzzySet(small);
  FuzzySet *safe = new FuzzySet(-20, 0, 0, 20);
  freqError->addFuzzySet(safe);
  FuzzySet *big = new FuzzySet(0, 20, 30, 50);
  freqError->addFuzzySet(big);
  fuzzy->addFuzzyInput(freqError);

  FuzzyOutput *putaran = new FuzzyOutput(1);
  FuzzySet *tuneup = new FuzzySet(-180, -130, -120, 0);
  putaran->addFuzzySet(tuneup);
  FuzzySet *intune = new FuzzySet(-120, 0, 0, 120);
  putaran->addFuzzySet(intune);
  FuzzySet *tunedown = new FuzzySet(0, 120, 130, 180);
  putaran->addFuzzySet(tunedown);
  fuzzy->addFuzzyOutput(putaran);

  // Building FuzzyRule "IF distance = small THEN speed = slow"
  FuzzyRuleAntecedent *ifDistanceSmall = new FuzzyRuleAntecedent();
  ifDistanceSmall->joinSingle(small);
  FuzzyRuleConsequent *thenSpeedSlow = new FuzzyRuleConsequent();
  thenSpeedSlow->addOutput(tuneup);
  FuzzyRule *fuzzyRule01 = new FuzzyRule(1, ifDistanceSmall, thenSpeedSlow);
  fuzzy->addFuzzyRule(fuzzyRule01);

  // Building FuzzyRule "IF distance = safe THEN speed = average"
  FuzzyRuleAntecedent *ifDistanceSafe = new FuzzyRuleAntecedent();
  ifDistanceSafe->joinSingle(safe);
  FuzzyRuleConsequent *thenSpeedAverage = new FuzzyRuleConsequent();
  thenSpeedAverage->addOutput(intune);
  FuzzyRule *fuzzyRule02 = new FuzzyRule(2, ifDistanceSafe, thenSpeedAverage);
  fuzzy->addFuzzyRule(fuzzyRule02);

  // Building FuzzyRule "IF distance = big THEN speed = high"
  FuzzyRuleAntecedent *ifDistanceBig = new FuzzyRuleAntecedent();
  ifDistanceBig->joinSingle(big);
  FuzzyRuleConsequent *thenSpeedFast = new FuzzyRuleConsequent();
  thenSpeedFast->addOutput(tunedown);
  FuzzyRule *fuzzyRule03 = new FuzzyRule(3, ifDistanceBig, thenSpeedFast);
  fuzzy->addFuzzyRule(fuzzyRule03);
}

void loop() {

  while (digitalRead(CHANGE_BUTTON) == HIGH && digitalRead(OK_BUTTON) == HIGH && digitalRead(BACK_BUTTON) == HIGH) {
  }

  if (digitalRead(CHANGE_BUTTON) == LOW && inSubMenu == false) {
    delay(50);
    while (digitalRead(CHANGE_BUTTON) == LOW)
      ;
    selectedItem = (selectedItem + 1) % itemCount;
    updateDisplay();
  }

  if (digitalRead(OK_BUTTON) == LOW && inSubMenu == false) {
    delay(50);
    while (digitalRead(OK_BUTTON) == LOW)
      ;
    handleMenuAction(selectedItem);
    delay(200);
  }

  if (digitalRead(BACK_BUTTON) == LOW && inSubMenu == false) {
    delay(50);
    while (digitalRead(BACK_BUTTON) == LOW)
      ;
    selectedItem = (selectedItem - 1 + itemCount) % itemCount;
    updateDisplay();
  }
}

void updateDisplay() {
  display.clearDisplay();

  // Display title
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("Pilih Mode/Senar"));

  // Handle scrolling
  if (selectedItem >= firstVisibleItem + 4) {
    firstVisibleItem = selectedItem - 3;  // Show 4 items at a time
  } else if (selectedItem < firstVisibleItem) {
    firstVisibleItem = selectedItem;
  }

  // Limit scrolling to avoid overflow
  if (firstVisibleItem < 0) {
    firstVisibleItem = 0;
  } else if (firstVisibleItem + 4 >= itemCount) {
    firstVisibleItem = itemCount - 4;
  }

  // Display menu items with cursor on the selected item
  for (int i = 0; i < 4; i++) {
    int menuItemIndex = firstVisibleItem + i;
    display.setCursor(0, 10 + i * 10);
    if (menuItemIndex >= 0 && menuItemIndex < itemCount) {
      if (menuItemIndex == selectedItem) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);  // Selected item
        display.print(">");
      } else {
        display.setTextColor(SSD1306_WHITE);
        display.print(" ");
      }
      display.print(menuItems[menuItemIndex]);
    }
  }
  display.display();
}

void handleMenuAction(int selectedItem) {
  // Implement actions for each menu item here
  inSubMenu = true;
  Serial.println(selectedItem);
  Serial.print("Selected item: ");
  Serial.println(menuItems[selectedItem]);
  // Add your actions based on the selected item
  switch (selectedItem) {
    case 0:
      checkEString();
      break;

    case 1:
      checkAString();
      break;

    case 2:
      checkDString();
      break;

    case 3:
      checkGString();
      break;

    case 4:
      checkBString();
      break;

    case 5:
      checkeString();
      break;

    default:
      //Nothing to do here
      break;
  }
  inSubMenu = false;
}

void checkBString() {
  while (digitalRead(BACK_BUTTON) == HIGH && inSubMenu == true) {
ulang:
    for (int i = 0; i < SAMPLES; i++) {
      microSeconds = micros();

      vReal[i] = analogRead(34);
      vImag[i] = 0;

      while (micros() < (microSeconds + samplingPeriod)) {
        delay(1);
      }
    }
    // Perform FFT on samples
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    // Find peak frequency and print peak
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    peak = peak - 509;
    if (peak > 50 || peak < -50 && digitalRead(BACK_BUTTON) == HIGH) {
      goto ulang;
    }
    display.clearDisplay();

    // Display title
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(F(" === Senar B === "));
    display.setCursor(0, 10);
    display.print(F(" Frekuensi Error: "));
    display.setCursor(0, 20);
    display.print(" ");
    display.print(peak);
    display.print(" Hz");
    Serial.println("\n\n\nEntrance: ");
    Serial.print("\t\t\tFrekuensi Error: ");

    Serial.println(peak);
    // Set the random value as an input
    fuzzy->setInput(1, peak);
    // Running the Fuzzification
    fuzzy->fuzzify();
    // Running the Defuzzification
    int output = fuzzy->defuzzify(1);
    // Printing something
    display.setCursor(0, 40);
    display.print(F(" Putaran: "));
    display.setCursor(0, 50);
    display.print(" ");
    display.print(output);
    display.print(" Step");
    Serial.println("Result: ");
    Serial.print("\t\t\tPutaran: ");
    Serial.println(output);
    myStepper.step(output);
    delay(500);
    display.display();

    if (digitalRead(BACK_BUTTON) == LOW && inSubMenu == true) {
      updateDisplay();
      inSubMenu = false;
      break;
    }
  }
}

void checkeString() {
  while (digitalRead(BACK_BUTTON) == HIGH && inSubMenu == true) {
ulang:
    for (int i = 0; i < SAMPLES; i++) {
      microSeconds = micros();

      vReal[i] = analogRead(34);
      vImag[i] = 0;

      while (micros() < (microSeconds + samplingPeriod)) {
        delay(1);
      }
    }
    // Perform FFT on samples
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    // Find peak frequency and print peak
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    peak = peak - 679;
    if (peak > 50 || peak < -50 && digitalRead(BACK_BUTTON) == HIGH) {
      goto ulang;
    }
    display.clearDisplay();

    // Display title
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(F(" === Senar e === "));
    display.setCursor(0, 10);
    display.print(F(" Frekuensi Error: "));
    display.setCursor(0, 20);
    display.print(" ");
    display.print(peak);
    display.print(" Hz");
    Serial.println("\n\n\nEntrance: ");
    Serial.print("\t\t\tFrekuensi Error: ");

    Serial.println(peak);
    // Set the random value as an input
    fuzzy->setInput(1, peak);
    // Running the Fuzzification
    fuzzy->fuzzify();
    // Running the Defuzzification
    int output = fuzzy->defuzzify(1);
    // Printing something
    display.setCursor(0, 40);
    display.print(F(" Putaran: "));
    display.setCursor(0, 50);
    display.print(" ");
    display.print(output);
    display.print(" Step");
    Serial.println("Result: ");
    Serial.print("\t\t\tPutaran: ");
    Serial.println(output);
    myStepper.step(output);
    delay(500);
    display.display();

    if (digitalRead(BACK_BUTTON) == LOW && inSubMenu == true) {
      updateDisplay();
      inSubMenu = false;
      break;
    }
  }
}

void checkGString() {
  while (digitalRead(BACK_BUTTON) == HIGH && inSubMenu == true) {
ulang:
    for (int i = 0; i < SAMPLES; i++) {
      microSeconds = micros();

      vReal[i] = analogRead(34);
      vImag[i] = 0;

      while (micros() < (microSeconds + samplingPeriod)) {
        delay(1);
      }
    }
    // Perform FFT on samples
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    // Find peak frequency and print peak
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    peak = peak - 808;
    if (peak > 50 || peak < -50 && digitalRead(BACK_BUTTON) == HIGH) {
      goto ulang;
    }
    display.clearDisplay();

    // Display title
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(F(" === Senar G === "));
    display.setCursor(0, 10);
    display.print(F(" Frekuensi Error: "));
    display.setCursor(0, 20);
    display.print(" ");
    display.print(peak);
    display.print(" Hz");
    Serial.println("\n\n\nEntrance: ");
    Serial.print("\t\t\tFrekuensi Error: ");

    Serial.println(peak);
    // Set the random value as an input
    fuzzy->setInput(1, peak);
    // Running the Fuzzification
    fuzzy->fuzzify();
    // Running the Defuzzification
    int output = fuzzy->defuzzify(1);
    // Printing something
    display.setCursor(0, 40);
    display.print(F(" Putaran: "));
    display.setCursor(0, 50);
    display.print(" ");
    display.print(output);
    display.print(" Step");
    Serial.println("Result: ");
    Serial.print("\t\t\tPutaran: ");
    Serial.println(output);
    myStepper.step(output);
    delay(500);
    display.display();

    if (digitalRead(BACK_BUTTON) == LOW && inSubMenu == true) {
      updateDisplay();
      inSubMenu = false;
      break;
    }
  }
}

void checkDString() {
  while (digitalRead(BACK_BUTTON) == HIGH && inSubMenu == true) {
ulang:
    for (int i = 0; i < SAMPLES; i++) {
      microSeconds = micros();

      vReal[i] = analogRead(34);
      vImag[i] = 0;

      while (micros() < (microSeconds + samplingPeriod)) {
        delay(1);
      }
    }
    // Perform FFT on samples
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    // Find peak frequency and print peak
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    peak = peak - 605;
    if (peak > 50 || peak < -50 && digitalRead(BACK_BUTTON) == HIGH) {
      goto ulang;
    }
    display.clearDisplay();

    // Display title
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(F(" === Senar D === "));
    display.setCursor(0, 10);
    display.print(F(" Frekuensi Error: "));
    display.setCursor(0, 20);
    display.print(" ");
    display.print(peak);
    display.print(" Hz");
    Serial.println("\n\n\nEntrance: ");
    Serial.print("\t\t\tFrekuensi Error: ");

    Serial.println(peak);
    // Set the random value as an input
    fuzzy->setInput(1, peak);
    // Running the Fuzzification
    fuzzy->fuzzify();
    // Running the Defuzzification
    int output = fuzzy->defuzzify(1);
    // Printing something
    display.setCursor(0, 40);
    display.print(F(" Putaran: "));
    display.setCursor(0, 50);
    display.print(" ");
    display.print(output);
    display.print(" Step");
    Serial.println("Result: ");
    Serial.print("\t\t\tPutaran: ");
    Serial.println(output);
    myStepper.step(output);
    delay(500);
    display.display();

    if (digitalRead(BACK_BUTTON) == LOW && inSubMenu == true) {
      updateDisplay();
      inSubMenu = false;
      break;
    }
  }
}

void checkAString() {
  while (digitalRead(BACK_BUTTON) == HIGH && inSubMenu == true) {
ulang:
    for (int i = 0; i < SAMPLES; i++) {
      microSeconds = micros();

      vReal[i] = analogRead(34);
      vImag[i] = 0;

      while (micros() < (microSeconds + samplingPeriod)) {
        delay(1);
      }
    }
    // Perform FFT on samples
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    // Find peak frequency and print peak
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    peak = peak - 680;
    if (peak > 50 || peak < -50 && digitalRead(BACK_BUTTON) == HIGH) {
      goto ulang;
    }
    display.clearDisplay();

    // Display title
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(F(" === Senar B === "));
    display.setCursor(0, 10);
    display.print(F(" Frekuensi Error: "));
    display.setCursor(0, 20);
    display.print(" ");
    display.print(peak);
    display.print(" Hz");
    Serial.println("\n\n\nEntrance: ");
    Serial.print("\t\t\tFrekuensi Error: ");

    Serial.println(peak);
    // Set the random value as an input
    fuzzy->setInput(1, peak);
    // Running the Fuzzification
    fuzzy->fuzzify();
    // Running the Defuzzification
    int output = fuzzy->defuzzify(1);
    // Printing something
    display.setCursor(0, 40);
    display.print(F(" Putaran: "));
    display.setCursor(0, 50);
    display.print(" ");
    display.print(output);
    display.print(" Step");
    Serial.println("Result: ");
    Serial.print("\t\t\tPutaran: ");
    Serial.println(output);
    myStepper.step(output);
    delay(500);
    display.display();

    if (digitalRead(BACK_BUTTON) == LOW && inSubMenu == true) {
      updateDisplay();
      inSubMenu = false;
      break;
    }
  }
}

void checkEString() {
  while (digitalRead(BACK_BUTTON) == HIGH && inSubMenu == true) {
ulang:
    for (int i = 0; i < SAMPLES; i++) {
      microSeconds = micros();

      vReal[i] = analogRead(34);
      vImag[i] = 0;

      while (micros() < (microSeconds + samplingPeriod)) {
        delay(1);
      }
    }
    // Perform FFT on samples
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    // Find peak frequency and print peak
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    peak = peak - 337;
    if (peak > 50 || peak < -50 && digitalRead(BACK_BUTTON) == HIGH) {
      goto ulang;
    }
    display.clearDisplay();

    // Display title
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(F(" === Senar B === "));
    display.setCursor(0, 10);
    display.print(F(" Frekuensi Error: "));
    display.setCursor(0, 20);
    display.print(" ");
    display.print(peak);
    display.print(" Hz");
    Serial.println("\n\n\nEntrance: ");
    Serial.print("\t\t\tFrekuensi Error: ");

    Serial.println(peak);
    // Set the random value as an input
    fuzzy->setInput(1, peak);
    // Running the Fuzzification
    fuzzy->fuzzify();
    // Running the Defuzzification
    int output = fuzzy->defuzzify(1);
    // Printing something
    display.setCursor(0, 40);
    display.print(F(" Putaran: "));
    display.setCursor(0, 50);
    display.print(" ");
    display.print(output);
    display.print(" Step");
    Serial.println("Result: ");
    Serial.print("\t\t\tPutaran: ");
    Serial.println(output);
    myStepper.step(output);
    delay(500);
    display.display();

    if (digitalRead(BACK_BUTTON) == LOW && inSubMenu == true) {
      updateDisplay();
      inSubMenu = false;
      break;
    }
  }
}
