#include "Arduino.h"
#include <SoftwareSerial.h>

#include "RFID.h"

SoftwareSerial rfid(RFID::Rx, RFID::Tx);

RFID::RFID() {
    rfid.begin(2400);

    pinMode(RFID::EPin, OUTPUT);
    digitalWrite(9, LOW);
}

bool RFID::detect() {
    digitalWrite(RFID::EPin, LOW);
    rfid.listen();
    if (rfid.available()) {
        while (rfid.available())
            rfid.read();

        digitalWrite(RFID::EPin, HIGH);
        return true;
    }

    return false;
}
