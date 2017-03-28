#include "Arduino.h"
#include "Servo.h"

#include "LineFollowing.h"

LineFollowing::LineFollowing() {
    Serial.begin(9600);
    while(!Serial);
    left.attach(PIN_SERVO_LEFT);
    right.attach(PIN_SERVO_RIGHT);
    speed(0.);
    Serial.print('b');

    pinMode(PIN_QTI_POWER_LEFT, OUTPUT);
    digitalWrite(PIN_QTI_POWER_LEFT, HIGH);
    pinMode(PIN_QTI_POWER_RIGHT, OUTPUT);
    digitalWrite(PIN_QTI_POWER_RIGHT, HIGH);
}

void LineFollowing::speed(double s) {
    speed(s, s);
}

void LineFollowing::speed(double l, double r) {
    left.write(1500 + 200 * l);
    right.write(1500 - 100 * r);
}

void LineFollowing::forward() {
    speed(1.);
    delay(obstacle_delay);

    long double t1, t2;

    while (1) {
        speed(1.);
        t1 = rc_time(PIN_QTI_RIGHT);
        t2 = rc_time(PIN_QTI_LEFT);

        if ((t1 > THRESHOLD) && (t2 > THRESHOLD)) {
            speed(0.);
            return;
        }
        else if (t1 > THRESHOLD && t2 < THRESHOLD)
            speed(-1., 1.);
        else if (t1 < THRESHOLD && t2 > THRESHOLD)
            speed(1., -1.);
    }

}

long double LineFollowing::rc_time(int pin) {
    long double duration = 0;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delay(1);
    pinMode(pin, INPUT);
    digitalWrite(pin, LOW);
    while (digitalRead(pin))
        duration++;

    return duration;
}
