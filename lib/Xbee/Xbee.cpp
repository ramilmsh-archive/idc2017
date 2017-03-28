#include "Arduino.h"
#include <SoftwareSerial.h>

#include "Xbee.h"

SoftwareSerial xbee(Xbee::Rx, Xbee::Tx);

Xbee::Xbee() {
    xbee.begin(9600);
    pinMode(Xbee::EPin, INPUT);
}

void Xbee::send(char* msg) {
    for (unsigned int i = 0; i < sizeof(msg); ++i)
        xbee.print(msg + i);
}

void Xbee::listen(char* buffer) {
    int c = 0;
    while (xbee.available())
        buffer[c++] = xbee.read();
}
