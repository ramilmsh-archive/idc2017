#include "Servo.h"

class LineFollowing {
private:
    Servo left, right;

    int turn_delay = 50;
    int forward_delay = 50;
    int obstacle_delay = 200;

    int PIN_QTI_POWER_LEFT = 3;
    int PIN_QTI_POWER_RIGHT = 6;

    int PIN_QTI_LEFT = 2;
    int PIN_QTI_RIGHT = 5;

    int PIN_SERVO_LEFT = 13;
    int PIN_SERVO_RIGHT = 12;

    int THRESHOLD = 50;

    void speed(double);
    void speed(double, double);

public:
    LineFollowing();
    void forward();

    static long double rc_time(int);
};
