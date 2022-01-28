#ifndef UWFM_H
#define UWFM_H

// Mapping from ADC<N> to physical pins.
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

// Delay between polling ADCs in milliseconds.
#define POLL_RATE 1000

namespace uwfm
{
    // Array of all ADC pins.
    static const uint8_t adcPins[10] = {
        ADC0, ADC1, ADC2, ADC3, ADC4,
        ADC5, ADC6, ADC7, ADC8, ADC9
    };

    /**
     * The calculations for the resistors and their correspondence with the 
     * current we expect to be going through each line can be found in the 
     * "Design Specifications.xlsx" file on Z drive (2020_master). For easier 
     * reference the current calculations are here.
     * 
     * Resistors were specced based off a 50V/V gain and V_max to the ADC of
     * 3V. Designing for 3V instead of the actual max of 3.3V of the ADC allows
     * some extra headroom in case we exceed current expectations.
     * 
     * Let's define the following variables:
     *  - r_shunt: [Ohms] The current sense resistor value
     *  - v_amp: [V] The voltage from the current sense amplifier (read by ADC)
     *  - gain: [V/V] Voltage multiplication factor of the amp - 50V/V specced
     *  - curr: [A] The current we calculate the load is drawing
     *  - steps: Number of bits used to digitize the input samples
     *  - adc_max: [V] Max input voltage to ADC - 3.3V specced
     * 
     * Here is the formula to calculate the current from the load:
     *  - curr = v_amp * adc_max / steps / gain / r_shunt
     * 
     * The description beside each resistor value corresponds to the following:
     * device [ nominal mOhms | nominal max A ][ actual mOhms | actual max A ]
     * 
     * Where:
     *  - nominal mOhms: The ideal resistor value for the current expected
     *  - nominal max A: The max current calculated at 3V (NOT 3.3V)
     *  - actual mOhms: The actual resistor specced for the load
     *  - actual max A: The max current calculated at 3.3V (NOT 3V)
     */
    static const double resistorValues[10] = {
                    // # DEVICE     [  NomO |  NomA ][  ActO | ActA  ]
        2.5e-3,     // 0 Fan        [   2.4 |  25.0 ][   2.5 | 26.40 ]
        15e-3,      // 1 Injector   [  25.0 |   4.0 ][  15.0 |  4.40 ]
        5e-3,       // 2 Fuel Pump  [   4.0 |  15.0 ][   5.0 | 13.20 ]
        12e-3,      // 3 Spark      [  12.0 |   5.0 ][  12.0 |  5.50 ]
        50e-3,      // 4 Water Pump [  46.2 |   1.3 ][  50.0 |  1.32 ]
        12e-3,      // 5 ECU        [  12.0 |   5.0 ][  12.0 |  5.50 ]
        120e-3,     // 6 DAQ        [ 120.0 |   0.5 ][ 120.0 |  0.55 ]
        12e-3,      // 7 Extra 12V  [  12.0 |   5.0 ][  12.0 |  5.50 ]
        120e-3,     // 8 5V Lines   [ 120.0 |   0.5 ][ 120.0 |  0.55 ]
        2e-3,       // 9 Alternator [   2.0 |  30.0 ][   2.0 | 33.00 ]
    };

    /**
     * Read analog voltage and calculate the current based off resistor value 
     * for the ADC index.
    */
    inline double calculateCurrent(uint8_t adc_idx) {
        double calc = analogRead(adcPins[adc_idx]);
        calc *= 3.3 / 4096 / 50;
        calc /= resistorValues[adc_idx];
        return calc;
    }

} // end namespace UWFM

#endif // UWFM_H
