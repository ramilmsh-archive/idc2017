#include "Arduino.h"
#include "SoftwareSerial.h"

#include "LineFollowing.h"
#include "RFID.h"

LineFollowing flw;
RFID tags;

void setup() {
    Serial.begin(9600);
    while (!Serial);
}

void loop() {
    Serial.println('v');
    delay(500);
}
