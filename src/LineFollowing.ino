#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial Xbee (10, 11);
SoftwareSerial RFID (0, 1);

int  val = 0;
char code[10];
int bytesread = 0;
byte state = 0;

byte count = 0;
bool a[5];

// Charge times
int ls1;
int ls2;

// threshold
int t = 50;
Servo left, right;

void setup() {
  // Starting serial
  Serial.begin(9600);
  Xbee.begin(9600);
  while (!Serial);

  left.attach(13);
  right.attach(12);

  // Switching QTI on
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
}

void loop() {

  ls1 = RCTime(5);
  ls2 = RCTime(2);

  Serial.print(ls2);
  Serial.print(' ');
  Serial.println(ls1);

  if ((ls1 > t) && (ls2 > t))
    speed(0);
  else if (ls1 > t && ls2 < t)
    speed(-.5, .5);
  else if (ls1 < t && ls2 > t)
    speed(.5, -.5);
  else
    speed(1);
}

// Set speed between -1 and 1
// l - left, r - right
void speed(double l, double r) {
  // TODO add calibration ?
  left.write(1500 + 200 * l);
  right.write(1500 - 100 * r);
  delay(l != r ? 100 : 50);
}

// Set forward speed
void speed(double s) {
  speed(s, s);
}

long RCTime(int pin) {
  long duration = 0;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delay(1);
  pinMode(pin, INPUT);
  digitalWrite(pin, LOW);
  while (digitalRead(pin))
    duration++;

  return duration;
}
