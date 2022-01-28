#include "uwfm.h"

static uint32_t last_poll = 0;

void setup() {
    // Set the serial port to a baud rate of 9600, this is used for printing
    // to the serial monitor/plotter in the Arduino IDE.
    Serial.begin(9600);

    // Change the analog read resolution to 12-bits. The default is 10-bits.
    analogReadResolution(12);
}

void loop() {
    // Use millis instead of delay. It simply returns the number of
    // milliseconds passed since the board began to run. Native type is
    // unsigned long, goes to 50 days. Most importantly, it doesn't block other
    // logic in this loop, it simply skips the items in the if statement.
    if ((millis() / POLL_RATE) != last_poll) {
        last_poll = millis() / POLL_RATE;

        // Loop through all analog inputs.
        for (uint8_t i = 0;
             i < (sizeof(uwfm::adcPins) / sizeof(uint8_t));
             i++)
        {
            double current = uwfm::calculateCurrent(i);
            Serial.print(current);
            Serial.print("\t");
        }

        Serial.println("\n");
    }
}
