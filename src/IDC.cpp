#include "Arduino.h"
#include "LineFollowing.h"

LineFollowing *flw;
void setup() {
    flw = new LineFollowing();
}

void loop() {
    flw->forward();
    delay(500);
}
