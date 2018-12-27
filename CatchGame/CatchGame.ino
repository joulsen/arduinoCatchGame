// GAME SETTINGS
float dropSpeedMultiplier = 1.0;
float dropSpeedDelta = 0.1;
const int maxDrops = 2;
const int collisionBuffer = 6;
const int bottomLineBuffer = 2;
int minDropDistance = 10;
int initialDropInterval = 5000;
const bool mute = false;

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
  bool reverse = false;
  bool mirror = false;
  int score = 0;
};

struct drop {
  int pos[2];
  unsigned long T0;
  int ySpeed = 150;
  char sprite;
};

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(9600);
  randomSeed(1500);
  pinMode(buttonPin, INPUT);
}

void loop() {
  int state = drawTitle();
  int score = startGame();
  Serial.print("GAME SCORE: ");
  Serial.print(score);
  delay(1000000000000000);
}
