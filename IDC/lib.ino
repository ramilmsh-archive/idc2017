// Delay when looping through an array
#define LCD_DELAY 100

#define Xbee_Rx 53
#define Xbee_Tx 51

#define LCD_Tx 10

#define BOT_NUMBER 3

SoftwareSerial Xbee(Xbee_Rx, Xbee_Tx);
SoftwareSerial LCD(255, LCD_Tx);

// The data from other bots
byte group[5];

// Global variables for score
int total_score;
int local_score;

// Variable that keeps checks if all the data was received
byte finished = 0;

// Starting Xbee
void setup_Xbee() {
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

// Printing string
void write(char* s) {
  LCD.write(12);
  LCD.write(s);
}

// Printing an integer
void write(int x) {
  LCD.write(12);
  LCD.print(x);
}

// Printing a character
void write(byte x) {
  LCD.write(12);
  LCD.print(x, BIN);
}

void write(int t, int l) {
  LCD.write(12);
  LCD.print("Total: ");
  LCD.print(t);
  LCD.write(13);
  LCD.print("Local: ");
  LCD.print(l);
}

// Calculating score
int calc_score() {
  group[1] = 31;
  Serial.println("--------------------");
  for (int i = 0; i < 5; ++i) {
    Serial.print(i);
    Serial.print(" :: ");
    Serial.println(group[i], BIN);
  }
  int score = 0;
  // Removing botnumber from data and writing it to the variable
  group[BOT_NUMBER] = data & 31;

  // Chaser:
  //  if snitch is caught - 1
  //  if not - 0
  score += (group[0] != 0) * 150;

  // Seekers:
  //  find intersection of both and add 10 points for each
  //  intersection found
  //
  // _t keeps all hashmarks that have intersection
  byte _t = group[1] & group[2];
  for (int i = 0; i < 5; ++i)
    // check if there is anything at a hashmark
    // if there is - add 10 points
    // if not - add 0 points
    score += (((1 << i) & _t) != 0) * 10;

  // Beaters:
  //  same as Seeker
  local_score = 0;
  _t = group[3] & group[4];
  for (int i = 0; i < 5; ++i)
    local_score += (((1 << i) & _t) != 0) * 10;
  
  score += local_score;

  LCD.write(12);
  LCD.print(score);
  LCD.write(13);
  LCD.print(data, BIN);

  return score;
}

void _send() {
  int count = 0;

  while (1) {
    // if not all the info has been received and there is
    // incoming signal
    if ((finished != 31) && Xbee.available()) {
      // reading the character
      byte incoming = Xbee.read(),
           // getting the number of the bot (first three bits)
           bot = incoming >> 5;

      if (bot > 4)
        continue;

      // getting only the info, removing the bot number
      incoming = incoming & 31;
      group[bot] = incoming;

      /*Serial.print(bot);
        Serial.print(" :: ");
        Serial.println(incoming, BIN);*/

      // recording that this bot has been received already
      finished = finished | (1 << bot);

      // calculating the score and displaying it
      total_score = calc_score();
      write(total_score, local_score);
    }

    // Sending data every 50 ms
    // !!! IMPORTANT !!!
    // DO NOT USE Xbee.print
    // Xbee write sends raw data, which is what we need
    Xbee.write(data);
    delay(200);
  }
}
