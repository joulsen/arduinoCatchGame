/* - - Catch Game for Arduino - -
 * Catch the falling objects to earn points. Miss a point and the game is over.
 * Controls using a potentiometer and displays using an I2C 128x64 OLED display.
 * Written 2018
 * by Andreas Just Poulsen
 * for AAU Imperative Programming Exam (Imperative Programming (IMPR) (EIT1, ITC1, ST1) - AAL E18 [E18-27624])
 * last modified: 16-03-2019
*/

// SETTINGS
// CURSOR
const int initCursorLength = 10;    // Initial length of cursor
const int initBottomLine = 63;      // Initial vertical position of cursor
const int captureBuffer = 1;        // Horizontal buffer for catching drops
// DROPS
const int maxDrops = 5;             // Max drops on screen
const float initDropSpeed = 0.008;  // Initial speed in pixels pr. milliseconds
const int initDropInterval = 3500;  // Initial interval between drops
const int elevationChange = 10;     // Elevation change when capturing up or down arrow
const int lengthChange = 5;         // Lenght change when capturing + or -
// MISC
const bool mute = false;            // Piezo speaker mute
// MENU
const int minReturnTime = 300;      // Minimum time spent on menu before being able to return

// PINS
const int potPin = A0;
const int buttonPin = 2;
const int piezoPin = 11;
const int mainFreq = 440;


// SCREEN STUFF
// FOR ADUINO UNO: SDA A4 and SCK A5
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
const int cWidth = 6; // Character width
const int cHeight = 8; // Character height
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// DEFINE GAME STRUCTS
struct cursor { // Cursor controlled by potentiometer
  unsigned int length = initCursorLength;
  unsigned int pos;
  unsigned int bottomline = initBottomLine;
  unsigned int score = 0;
  bool playing = true;
};

struct drop { // Drop on screen
  bool enabled = false;
  unsigned int born;    // Time when created
  unsigned int pos[2];  // X- and Y-position
  float ySpeed = initDropSpeed; // Speed in pixels pr. millisecond
  char sprite; // Character to display as drop
};

// For menu browsing
unsigned int windowStart;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(9600);
  randomSeed(analogRead(A1)); // Use analog port A1 for random seed
  pinMode(buttonPin, INPUT);
}

void loop() {
  switch (drawTitle()) { // Draw title
    case 0: // If help selected
      drawHelp();
      break;
    case 1: // If play selected
      int score = catchGame();
      showScore(score);
      break;
    default: Serial.println("ERROR: Unexpected drawTitle output");
  }
}
