float currentVals[10]{};

// Populate with Resistor Values
float resistorValues[10] = {10,10,10,10,10,10,10,10,10,10};

#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A6 6
#define A7 7
#define A8 8
#define A9 9

int analogPins[10] = {
    A0, A1, A2, A3, A4,
    A5, A6, A7, A8, A9
};

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Assumes that Analog Pins A0-A9 are used for input
  for (int i{}; i < 10; i++){
    // Convert analog value to corresponding voltage
    currentVals[i] = analogRead(i) * (5.0 / 1023.0);

    // Current calculation using V=IR
    currentVals[i] /= resistorValues[i];
    
    Serial.print(currentVals[i]);
    Serial.print("\t");
  }
  Serial.print("\n\n");
  delay(1000);
}