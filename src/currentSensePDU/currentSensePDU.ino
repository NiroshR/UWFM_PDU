// University of Waterloo Formula Motorsports
// Electronics Team
// Current Sense PDU
// Season: 2022
// Owner: Nirosh Ratnarajah
// Last Revised: March 16, 2022

// MoTeC Resource: https://youtu.be/2m34nQ4i6KQ

#include <arduino.h>
#include <eXoCAN.h>
#include <uwfm.h>

#define BOARD1
#define DEBUG
#define NUM_FRAMES 3

MSG rxMsg;                                       // RX from other CAN devices
msgFrm frames[NUM_FRAMES];                       // 3 TX groups (10 ADCs)
eXoCAN can(STD_ID_LEN, BR250K, PORTB_8_9_XCVR);  // constuctor for eXoCAN lib
char cBuff[20];                                  // holds formatted string

enum TxGroups : uint8_t {
    Group1 = 0,  // Fan, Inj, Fuel Pump, Spark
    Group2 = 1,  // Water Pump, ECU, DAQ, Bus 12V
    Group3 = 2   // Bus 5V, Alternator
};

#ifdef BOARD1
enum TxCanIds : u_int8_t {
    adrGroup1 = 0xAD,
    adrGroup2,
    adrGroup3
};  // 0xAD - 0xAF
#endif
#ifdef BOARD2
enum TxCanIds : u_int8_t {
    adrGroup1 = 0xE8,
    adrGroup2,
    adrGroup3
};  // 0xE8 - 0xEA
#endif

#ifdef BOARD1
enum RxFilterIds : u_int8_t {
    adrRxGroup1 = 0xE8,
    adrRxGroup2,
    adrRxGroup3
};  // 0xE8 - 0xEA
#endif
#ifdef BOARD2
enum RxFilterIds : u_int8_t {
    adrRxGroup1 = 0xAD,
    adrRxGroup2,
    adrRxGroup3
};  // 0xAD - 0xAF
#endif

void canTX(msgFrm frm, bool prt = false)  // send a CAN Bus message
{
    uint8_t msgLen = frm.txMsgLen;
    can.transmit(frm.txMsgID, frm.txMsg.bytes, msgLen);

#ifdef DEBUG
    static uint16_t buff[4];
    memcpy(buff, frm.txMsg.bytes, sizeof(buff));
    sprintf(cBuff, "tx @%02x #%d \t", frm.txMsgID, msgLen);
    Serial.print(cBuff);
    for (int j = 0; j < (msgLen % 2); j++) {
        sprintf(cBuff, "%6d ", buff[j]);
        Serial.print(cBuff);
    }
    Serial.println();
#endif
}

uint8_t canRX()  // check for a CAN Bus message
{
    int len, id = 0, fltIdx;
    len = can.receive(id, fltIdx, rxMsg.bytes);

#ifdef DEBUG
    static uint16_t buff[4];
    memcpy(buff, rxMsg.bytes, sizeof(buff));
    if (len >= 0) {
        sprintf(cBuff, "RX @%02x #%d  %d\t", id, len, fltIdx);
        Serial.print(cBuff);
        for (int j = 0; j < (len / 2); j++) {
            sprintf(cBuff, "%6d ", buff[j]);
            Serial.print(cBuff);
        }
        Serial.println();
    }
#endif

    return id;
}

void setup() {
    // Init serial print and have ADCs configured to 12 bits of precision.
    Serial.begin(9600);
    analogReadResolution(12);

    /*

     .-----------------------------------------------------------------------.
     | Byte 1 | Byte 2 | Byte 3 | Byte 4 | Byte 5 | Byte 6 | Byte 7 | Byte 8 |
     |-----------------------------------------------------------------------|
     |       Fan       |    Injectors    |    Fuel Pump    |      Spark      |
     |-----------------------------------------------------------------------|
     |    Water Pump   |       ECU       |       DAQ       |     Bus 12V     |
     |-----------------------------------------------------------------------|
     |      Bus 5V     |    Alternator   |              not used             |
     '-----------------------------------------------------------------------'

    */

    // Frames specify the following:
    //  - adr: TX ID on CAN line
    //  - id type: length of ID, 9 is standard and 11 is extended
    //  - len: number of data bytes
    //  - data: zero init data
    //  - hw config: PORTB_8_9_XCVR is pins B8/B9 with external CAN transceiver
    //  - delay: time in milliseconds between transmission of each frame
    frames[Group1] = {adrGroup1, STD_ID_LEN, 0x08, {0x00}, PORTB_8_9_XCVR, 250};
    frames[Group2] = {adrGroup2, STD_ID_LEN, 0x08, {0x00}, PORTB_8_9_XCVR, 250};
    frames[Group3] = {adrGroup3, STD_ID_LEN, 0x04, {0x00}, PORTB_8_9_XCVR, 250};

    // Whitelist of CAN IDs that we receive from.
    can.filterList16Init(0, adrRxGroup1, adrRxGroup2, adrRxGroup3);

    // Currently not using CAN interrupts, having issues when there is a very
    // high RX rate and printing to serial bus. In reality, we only need to
    // receive messages from DAQ and even so, we wouldn't do anything with it.

    //  can.attachInterrupt(canRX);
}

uint32_t lastT[NUM_FRAMES] = {0};  // last time we transmitted per frame
void loop() {
    for (uint8_t n = Group1; n <= Group3; n++) {
        // Send out message if we have surpassed the specified delay.
        // Using `millis()` is non-blocking compared to `delay()`.
        if (millis() / frames[n].txDly != lastT[n]) {
            switch (n) {
            case Group1:
                for (size_t i = 0; i < 4; i++) {
                    frames[Group1].txMsg.uint16[i] =
                        uwfm::calculateCurrent(n * 4 + i);
                }
                break;
            case Group2:
                for (size_t i = 0; i < 4; i++) {
                    frames[Group2].txMsg.uint16[i] =
                        uwfm::calculateCurrent(n * 4 + i);
                }
                break;
            case Group3:
                for (size_t i = 0; i < 2; i++) {
                    frames[Group3].txMsg.uint16[i] =
                        uwfm::calculateCurrent(n * 4 + i);
                }
                break;
            default:
                break;
            }

            // Log time we send out this CAN message
            lastT[n] = millis() / frames[n].txDly;

            // Send out single CAN frame.
            canTX(frames[n]);
        }
    }

    // Recieve messages from RX buffer.
    canRX();
}