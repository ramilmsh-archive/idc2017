#include "Servo.h"
#include "SoftwareSerial.h"

#define RFID_OUT 33
#define RFID_5V 31
#define RFID_GROUND 35

#define LEFT_OUT 39
#define LEFT_5V 37
#define LEFT_GROUND 41

#define RIGHT_OUT 45
#define RIGHT_5V 43
#define RIGHT_GROUND 47

#define SERVO_PIN_LEFT 13
#define SERVO_PIN_RIGHT 12

#define LIMIT 150
#define THRESHOLD 50
#define RFID_THRESHOLD 150

#define PIEZO_PIN 9

Servo left, right;
byte data;

void setup() {
  // Attaching servos
  left.attach(SERVO_PIN_LEFT);
  right.attach(SERVO_PIN_RIGHT);

  // Starting Serial
  Serial.begin(9600);
  while (!Serial);
  setup_LCD();

  // Left QTI
  pinMode(LEFT_5V, OUTPUT);
  digitalWrite(LEFT_5V, HIGH);
  pinMode(LEFT_GROUND, OUTPUT);
  digitalWrite(LEFT_GROUND, LOW);

  // Right QTI
  pinMode(RIGHT_5V, OUTPUT);
  digitalWrite(RIGHT_5V, HIGH);
  pinMode(RIGHT_GROUND, OUTPUT);
  digitalWrite(RIGHT_GROUND, LOW);

  // RFID
  pinMode(RFID_5V, OUTPUT);
  digitalWrite(RFID_5V, HIGH);
  pinMode(RFID_GROUND, OUTPUT);
  digitalWrite(RFID_GROUND, LOW);

  // PIEZO
  pinMode(PIEZO_PIN, OUTPUT);
  for (int i = 0; i < 5; ++i) {
    forward();
    data = data | (detect() << i);
  }

  left.detach();
  right.detach();

  calc_score();
  setup_Xbee();
  _send();
}

void loop() {}

void forward() {
  write("following");
  speed(1.);
  long ls1 = 0;
  long ls2 = 0;

  while ((ls1 < THRESHOLD) || (ls2 < THRESHOLD)) {
    ls1 = rc_time(LEFT_OUT);
    ls2 = rc_time(RIGHT_OUT);
    
    /*Serial.print(ls1);
    Serial.print(' ');
    Serial.println(ls2);*/
    
    if (ls1 > THRESHOLD && ls2 < THRESHOLD)
      speed(-.5, 1);
    else if (ls1 < THRESHOLD && ls2 > THRESHOLD)
      speed(1, -.5);
    else
      speed(1);
  }
}

boolean detect() {
  // Serial.println("------------------------");
  write("detecting");
  int c = 0;
  long t;
  while (c++ < LIMIT) {
    t = rc_time(RFID_OUT);
    // Serial.println(t);
    if (t > RFID_THRESHOLD) {
      write("found");
      delay(150);
      return 1;
    }
  }

  write("");
  return 0;
}

// Set speed between -1 and 1
// l - left, r - right
void speed(double l, double r) {
  left.writeMicroseconds(1500 + 200 * l);
  right.writeMicroseconds(1500 - 100 * r);
}

// Set forward speed
void speed(double s) {
  speed(s, s);
}

long rc_time(int pin) {
  long d = 0;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delay(1);
  pinMode(pin, INPUT);
  digitalWrite(pin, LOW);

  while (digitalRead(pin) && (d <= RFID_THRESHOLD + 1))
    d++;

  return d;
}
