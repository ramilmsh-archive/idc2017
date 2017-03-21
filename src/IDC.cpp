#include "Arduino.h"

#include "LineFollowing.h"
#include "RFID.h"

LineFollowing flw;
RFID rfid;
void setup() {
}

void loop() {
    flw.forward();
    rfid.detect();
    delay(500);
}
