// Delay when looping through an array
#define LCD_DELAY 100
#define Xbee_Rx 12
#define Xbee_Tx 13

#define BOT_NUMBER 3

// Pins for your LCD display
//                  A   B   C   D   E   F   G   H
int LCD_PINS[8] = { 0,  0,  0,  0,  0,  0,  0,  0};

bool numbers[10][8] = {
  // 0
  {1, 1, 1, 1, 1, 1, 0, 0},
  // 1
  {0, 1, 1, 0, 0, 0, 0, 0},
  // 2
  {1, 1, 0, 1, 1, 0, 1, 0},
  // 3
  {1, 1, 1, 1, 0, 0, 1, 0},
  // 4
  {0, 1, 1, 0, 0, 1, 1, 0},
  // 5
  {1, 0, 1, 1, 0, 1, 1, 0},
  // 6
  {1, 0, 1, 1, 1, 1, 1, 0},
  // 7
  {1, 1, 1, 0, 0, 0, 0, 0},
  // 8
  {1, 1, 1, 1, 1, 1, 1, 0},
  // 9
  {1, 1, 1, 1, 0, 1, 1, 0}
};

bool _L[8] = {0, 0, 0, 1, 1, 1, 0, 0};
bool _H[8] = {0, 1, 1, 0, 1, 1, 1, 0};
bool _F[8] = {1, 0, 0, 0, 1, 1, 1, 0};
bool _[8]  = {0, 0, 0, 0, 0, 0, 0, 0};

SoftwareSerial Xbee(Xbee_Rx, Xbee_Tx);

// The data from other bots
byte group[5];

// Global variables for score
int score;
int a[3];

// Variable that keeps checks if all the data was received
byte finished = 0;

void setup_display() {
  for (int i = 0; i < 8; ++i)
    pinMode(LCD_PINS[i], OUTPUT);
}

void setup_Xbee() {
  // The data variable
  // Bot number written into it
  data = BOT_NUMBER << 5;
  
  // Starting Xbee
  Xbee.begin(2400);
  while (!Xbee);
}

// Raw display method, to display an array of bool
// where i-th element represents i-th LED
void _display(bool* a) {
  for (int i = 0; i < 8; ++i)
    digitalWrite(LCD_PINS[i], a[i] ? HIGH : LOW);
}

// Method to display an integer
void _display(int n) {
  _display(numbers[n]);
}

// Method to display a character {'L', 'H', 'F', ' '}
// 'L' - LineFollowing
// 'H' - HashMark
// 'F' - Object Detected
// ' ' - Clear Screen
void _display(char c) {
  switch (c) {
    case 'H':
      _display(_H);
      break;

    case 'L':
      _display(_L);
      break;

    case 'F':
      _display(_F);
      break;

    case ' ':
      _display(_);
      break;

    default:
      break;
  }
}

int calc_score() {
  score = 0;
  score += group[0] * 150;

  byte _t = group[1] & group[2];
  for (int i = 0; i < 5; ++i)
    score += ((1 << i) & _t) * 10;


  _t = group[3] & group[4];
  for (int i = 0; i < 5; ++i)
    score += ((1 << i) & _t) * 10;

  return score;
}

void send() {
  int count = 0;

  while (1) {
    if ((finished != 31) && Xbee.available()) {
      byte incoming = Xbee.read(),
           bot = incoming >> 5;

      incoming = incoming & 31;
      group[bot] = incoming;

      finished = finished | (1 << bot);
      if (finished == 31) {
        calc_score();

        for (int i = 2; i >= 0; --i) {
          a[i] = score % 10;
          score /= 10;
        }
      }
    }

    if (finished == 31) {
      _display(a[count++]);
      delay(LCD_DELAY);
      count = count == 3 ? 0 : count;
    }

    Xbee.print(data);
  }
}
