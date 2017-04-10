// Delay when looping through an array
#define LCD_DELAY 100

#define Xbee_Rx 53
#define Xbee_Tx 51
#define Xbee_5V 3

#define LCD_Tx 10

#define BOT_NUMBER 3

SoftwareSerial Xbee(Xbee_Rx, Xbee_Tx);
SoftwareSerial LCD(255, LCD_Tx);

// The data from other bots
byte group[5];

// Global variables for score
int total_score;

// Variable that keeps checks if all the data was received
byte finished = 0;

void setup_Xbee() {
  // Powering up the Xbee
  pinMode(Xbee_5V, OUTPUT);
  digitalWrite(Xbee_5V, HIGH);

  // The data variable
  // Bot number written into it
  data = data | BOT_NUMBER << 5;

  // Starting Xbee
  Xbee.begin(9600);
  while (!Xbee);
}

void setup_LCD() {
  LCD.begin(9600);
  while (!LCD);
  LCD.write(12);
}

void write(char* s) {
  LCD.write(12);
  LCD.write(s);
}

void write(int x) {
  LCD.write(12);
  LCD.print(x);
}

void write(byte x) {
  LCD.write(12);
  LCD.print(x, BIN);
}

int calc_score() {
  int score = 0;
  group[BOT_NUMBER] = data & 31;
  score += group[0] * 150;

  byte _t = group[1] & group[2];
  for (int i = 0; i < 5; ++i)
    score += (((1 << i) & _t) != 0) * 10;

  _t = group[3] & group[4];
  for (int i = 0; i < 5; ++i)
    score += (((1 << i) & _t) != 0) * 10;

  // TODO remove
  LCD.write(12);
  LCD.print(score);
  LCD.write(13);
  LCD.print(data, BIN);

  return score;
}

void _send() {
  int count = 0;

  while (1) {
    if ((finished != 31) && Xbee.available()) {
      char incoming = Xbee.read(),
           bot = incoming >> 5;

      Serial.println(incoming);

      incoming = incoming & 31;
      group[bot] = incoming;

      finished = finished | (1 << bot);
      total_score = calc_score();
      write(total_score);
    }
    //Xbee.print((char) data);
    delay(50);
  }
}
