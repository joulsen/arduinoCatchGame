// GAME SETTINGS
const float dropSpeedInit = 0.008;
int dropSpeedExtra = 0;
int dropSpeedDelta = 10;
const int maxDrops = 2;
const int collisionBuffer = 6;
const int bottomLineBuffer = 2;
int minDropDistance = 10;
int initialDropInterval = 5000;
const int lineWidenAmount = 2;
const int bottomLineChange = 1;
const int highestBottomLine = 100;
const bool mute = false;
const bool debugMode = true;

// MENU SETTINGS
const int minReturnTime = 300;

// PINS
const int potPin = A0;
const int buttonPin = 2;
const int piezoPin = 11;
const int mainFreq = 440;

// SCREEN STUFF
// FOR ADUINO UNO: SDA A4 and SCK A5
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
const int cWidth = 6;
const int cHeight = 8;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DEFINE GAME STRUCTS
struct cursor {
  int length = 10;
  int potField[2] = {0, 1023};
  int pos;
  int bottomline = SCREEN_HEIGHT - 1;
  int score = 0;
};

struct drop {
  int pos[2];
  unsigned int T0;
  float ySpeed = dropSpeedInit;
  char sprite;
};

unsigned int windowStart;
unsigned int T0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(9600);
  randomSeed(analogRead(A1));
  pinMode(buttonPin, INPUT);
}

void loop() {
  switch (drawTitle()) {
    case 0:
      drawHelp();
      break;
    case 1:
      int score = startGame();
      showScore(score);
      break;
  }
}
