drop drops[maxDrops];
int dropCount = 0;
int dropQueue = 0;

// Generate drop
struct drop genDrop() {
  drop drop;
  drop.pos[0] = random(0, SCREEN_WIDTH);
  drop.pos[1] = 0;
  switch (random(0, 10)) {
    case 0: drop.sprite = char(29); break;
    case 1: drop.sprite = char(19); break;
    default: drop.sprite = '*';
  }
  drop.T0 = millis();
  return drop;
}

int dropTimer;
bool dropTimerOn = false;
bool genDropTimed(int delta) {
  if (dropTimerOn) {
    if (dropTimer + delta < millis()) {
      dropTimerOn = false;
      drops[dropQueue] = genDrop();
      return true;
    }
  } else {
    dropTimerOn = true;
    dropTimer = millis();
  }
  return false;
}

void genDrops(struct drop drops[]) {
  if (dropCount != maxDrops) {
    if (genDropTimed(initialDropInterval)) {
      dropCount++;
      dropQueue++;
    }
  } else if (dropCount != dropQueue and dropQueue != -1) {
    drops[dropQueue] = genDrop();
    dropQueue = -1;
  }
}

int checkCollision(struct drop *drop, struct cursor *cursor) {
  if (drop->pos[1] <= cursor->bottomline + bottomLineBuffer and drop->pos[1] >= cursor->bottomline) {
    if (cursor->pos - collisionBuffer <= drop->pos[0] and drop->pos[0] <= cursor->pos + cursor->length + collisionBuffer) {
      buzz('H');
      return 1;
    } else {
      return -1;
    }
  } else {
    return 0;
  }
}

int handleCollision(struct drop drop, struct cursor *cursor) {
  switch (drop.sprite) {
    case '*':
      cursor->score += 1;
      return 0;
    case char(29):
      cursor->length += lineWidenAmount;
      return 1;
    case char(19):
      return -1;
    default:
      Serial.println("Error: Unexpected sprite in handleCollision!");
  }
}

// UPDATE POSITIONS AND DRAW GRAPHICS
void updateDrop(struct drop *drop, int bottomline) {
  display.fillRect(drop->pos[0], drop->pos[1], cWidth, cHeight, 0);
  drop->pos[1] = (millis() - drop->T0) / (drop->ySpeed / dropSpeedMultiplier);
  display.drawChar(drop->pos[0], drop->pos[1], drop->sprite, 1, 0, 1);
}

void updateCursor(struct cursor *cursor) {
  display.fillRect(0, cursor->bottomline, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  cursor->pos = map(analogRead(potPin), 0, 1023, SCREEN_WIDTH - cursor->length, 0);
  display.drawLine(cursor->pos, cursor->bottomline, cursor->pos + cursor->length, cursor->bottomline, 1);
  //display.display();
}

void drawScore(struct cursor cursor) {
  int score = cursor.score;
  display.fillRect(SCREEN_WIDTH - 5 * cWidth - 1, 0, 5 * cWidth + 1, 2 * cHeight, 1);
  display.setCursor(SCREEN_WIDTH - 5 * cWidth, 1);
  display.setTextColor(0);
  display.setTextSize(1);
  display.print("SCORE");
  display.setCursor(SCREEN_WIDTH - 5 * cWidth, cHeight + 1);
  display.print(cursor.score);
  //display.display();
}

// MAIN GAME LOOP
int startGame() {
  //drop drops[maxDrops];
  //dropCount = 0;
  //dropQueue = 0;
  display.clearDisplay();
  display.display();
  buzz('S');
  cursor cursor;
  int last_drop = millis();
  while (true) {
    if (debugMode) {
      debug(cursor, drops);
    }
    genDrops(drops);
    for (int i = 0; i < dropCount; i++) {
      updateDrop(&drops[i], cursor.bottomline);
      if (checkCollision(&drops[i], &cursor) == 1) {
        if (handleCollision(drops[i], &cursor) == -1) {
          buzz('M');
          return cursor.score;
        }
        dropQueue = i;
        dropSpeedMultiplier += dropSpeedDelta;
      } else if (checkCollision(&drops[i], &cursor) == -1) {
        if (handleCollision(drops[i], &cursor) != -1) {
          buzz('M');
          return cursor.score;
        }
        dropQueue = i;
      }
      /*
        if (checkCollision(&drops[i], &cursor) == 1) {
        if (handleCollision(drops[i], &cursor) == -1)  {
          return cursor.score;
          buzz('M');
        } else {
          dropQueue = i;
          dropSpeedMultiplier += dropSpeedDelta;
        }
        } else if (checkCollision(&drops[i], &cursor) == -1) {
        if (handleCollision(drops[i], &cursor) != -1) {
          return cursor.score;
          dropQueue = i;
        } else {

        }
        }*/
    }
    updateCursor(&cursor);
    drawScore(cursor);
    display.display();
  }
}

// BUZZ FUNCTION FOR PIEZO SPEAKER
void buzz(char mode) {
  if (!mute) {
    switch (mode) {
      case 'H':
        tone(piezoPin, 440, 100);
        break;
      case 'M':
        tone(piezoPin, 196, 100);
        delay(150);
        tone(piezoPin, 196, 250);
        break;
      case 'S':
        int tones[3] = {880, 932, 988};
        for (int i = 0; i < 3; i++) {
          tone(piezoPin, tones[i], 100);
          delay(100);
        }
        break;
    }
  }
}

// DEBUG FUNCTION

void debug(struct cursor cursor, struct drop drops[]) {
  Serial.print(millis());
  Serial.print(',');
  Serial.print(cursor.pos);
  for (int i = 0; i < dropCount; i++) {
    Serial.print(',');
    Serial.print(drops[i].pos[0]);
    Serial.print(',');
    Serial.print(drops[i].pos[1]);
  }
  Serial.println();
}
