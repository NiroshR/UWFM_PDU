#define ADC0 PA0
#define ADC1 PA1
#define ADC2 PA2
#define ADC3 PA3
#define ADC4 PA4
#define ADC5 PA5
#define ADC6 PA6
#define ADC7 PA7
#define ADC8 PB0
#define ADC9 PB1

// unsigned char: 8 bits (0-255)
static const unsigned char analogPins[10] = {
    ADC0, ADC1, ADC2, ADC3, ADC4,
    ADC5, ADC6, ADC7, ADC8, ADC9
};

static const double resistorValues[10] = {
    10.0, 10.0, 10.0, 10.0, 10.0,
    10.0, 10.0, 10.0, 10.0, 10.0
};

void setup() {
  Serial.begin(9600);
}

void loop() {
    double currentVals[10] = { 0 };

    // Assumes that Analog Pins A0-A9 are used for input
    for (int i = 0; i < (sizeof(analogPins) / sizeof(unsigned char)); i++){
        // Convert analog value to corresponding voltage
        currentVals[i] = analogRead(analogPins[i]) * (5.0 / 1023.0);

        // Current calculation using V=IR
        currentVals[i] /= resistorValues[i];

        Serial.print(currentVals[i]);
        Serial.print("\t");
    }

    Serial.println("\n");
    delay(1000);
}