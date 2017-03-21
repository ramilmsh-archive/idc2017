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
  switch (state) {
    // Line following
    case 0:
      ls1 = RCTime(5);
      ls2 = RCTime(2);

      Serial.print(ls2);
      Serial.print(' ');
      Serial.println(ls1);

      if ((ls1 > t) && (ls2 > t))
        speed(0);
      else {
        if (ls1 > t && ls2 < t)
          speed(-.5, .5);
        else if (ls1 < t && ls2 > t)
          speed(.5, -.5);
        else
          speed(1);

        break;
      }

    // RFID
    case 1:
      Serial.println("blabla");
      RFID.listen();
      // RFID
      if (RFID.available() == 10) {
        if ((val = RFID.read()) == 10) {
          bytesread = 0;
          while ((bytesread < 10) && (RFID.available() > 0)) {
            val = RFID.read();

            if ((val == 10) || (val == 13))
              break;

            code[bytesread] = val;
            bytesread++;
          }

          if (bytesread == 10) {
            a[count] = 1;
            Serial.print("TAG code is: ");
            Serial.println(code);
          }

          bytesread = 0;
          digitalWrite(12, HIGH);
          delay(1500);
          digitalWrite(12, LOW);
        }
      }

      // if all RFIDs counted communicate
      state = (count == 5) ? 2 : 0;
      count++;
      speed(1);
      delay(50);
      break;

    // TRANSMISSION
    case 2:
      speed(0);
      Xbee.listen();

      // OUTGOING
      if (Serial.available())
        Xbee.print(Serial.read());

      // INCOMING
      if (Xbee.available())
        Serial.println((char) Xbee.read());
      break;

    default:
      break;
  }
}
