#include <LineFollowing.h>
#include <Arduino.h>
#include <Servo.h>

LineFollowing::LineFollowing() {
    Servo left, right;
    left.attach(13);
    right.attach(12);
}

LineFollowing::speed(double s) {

}

LineFollowing::speed(double l, double r) {

}
