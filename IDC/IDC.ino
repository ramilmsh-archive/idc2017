#include "Servo.h"
#include "SoftwareSerial.h"

#define RFID_IN 31
#define RFID_POWER 33

#define SERVO_PIN_LEFT 13
#define SERVO_PIN_RIGHT 12

#define QTI_PIN_LEFT 5
#define QTI_PIN_RIGHT 2

#define QTI_POWER_PIN_LEFT 6
#define QTI_POWER_PIN_RIGHT 3

#define LED_GREEN 7
#define LED_RED 8

#define LIMIT 100
#define THRESHOLD 50
#define RFID_THRESHOLD 150

#define PIEZO_PIN 9

Servo left, right;
byte data;

void setup() {
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  
  // Attaching servos
  left.attach(SERVO_PIN_LEFT);
  right.attach(SERVO_PIN_RIGHT);

  // Starting Serial
  Serial.begin(9600);
  while (!Serial);

  setup_Xbee();
  setup_display();

  // Line Following QTI
  pinMode(QTI_POWER_PIN_LEFT, OUTPUT);
  pinMode(QTI_POWER_PIN_RIGHT, OUTPUT);
  digitalWrite(QTI_POWER_PIN_LEFT, HIGH);
  digitalWrite(QTI_POWER_PIN_RIGHT, HIGH);

  // RFID
  pinMode(RFID_POWER, OUTPUT);
  digitalWrite(RFID_POWER, HIGH);

  // PIEZO
  pinMode(PIEZO_PIN, OUTPUT);
  for (int i = 0; i < 5; ++i) {
    forward();
    data = data | (detect() << i);
  }

  speed(0.);
  play();
}

void loop() {}

void forward() {
  speed(1.);
  long ls1 = 0;
  long ls2 = 0;

  while ((ls1 < THRESHOLD) || (ls2 < THRESHOLD)) {
    ls1 = rc_time(QTI_PIN_LEFT);
    ls2 = rc_time(QTI_PIN_RIGHT);
    /*Serial.print(ls1);
      Serial.print(' ');
      Serial.println(ls2);
    */
    if (ls1 > THRESHOLD && ls2 < THRESHOLD)
      speed(.1, 1);
    else if (ls1 < THRESHOLD && ls2 > THRESHOLD)
      speed(1, .1);
    else
      speed(1);
  }
}

boolean detect() {
  Serial.println("------------------------");
  digitalWrite(LED_GREEN, HIGH);
  int c = 0;
  long t;
  while (c++ < LIMIT) {
    t = rc_time(RFID_IN);
    Serial.println(t);
    if (t > RFID_THRESHOLD) {
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
      delay(150);
      digitalWrite(LED_RED, LOW);
      return 1;
    }
  }

  digitalWrite(LED_GREEN, LOW);
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

  while (digitalRead(pin))
    d++;

  return d;
}





