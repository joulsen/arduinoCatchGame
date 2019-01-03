unsigned int lastDrop;
unsigned int dropInterval = initDropInterval;

// Function for generating sprites for, X-positions of and enabling drops.
struct drop genDrop() {
  drop drop;
  drop.pos[0] = random(0, SCREEN_WIDTH - cWidth);
  drop.pos[1] = 0;
  switch (random(0, 20)) {
    case 0: drop.sprite = '$';      break;
    case 1 or 2: drop.sprite = '+'; break;
    case 3: drop.sprite = '-';      break;
    case 4: drop.sprite = char(24); break;
    case 5: drop.sprite = char(25); break;
    case 6: drop.sprite = char(19); break;
    default: drop.sprite = '*';
  }
  drop.born = millis(); // This value is used in calculating position
  drop.enabled = true;
  return drop;
}

// Ensures no drops are spawned ontop of eachother
void spawnDrops(struct drop drops[], int dropCount) {
  for (int i = 0; i < dropCount; i++) {
    unsigned int cT = millis();
    if (!drops[i].enabled and cT - lastDrop > dropInterval) {
      drops[i] = genDrop();
      lastDrop = millis();
    }
  }
}

// Checks collision and reacts accordingly
void checkCollision(struct drop *drop, struct cursor *cursor) {
  if (drop->pos[1] > cursor->bottomline) { // If drop goes under cursor
    if (drop->enabled) {
      // If drop is in the same horizontal space as the cursor
      if (cursor->pos - captureBuffer <= drop->pos[0] and drop->pos[0] <= cursor->pos + cursor->length + captureBuffer) {
        buzz('H');
        switch (drop->sprite) { // Do something according to sprite of the drop
          case '*': cursor->score += 1;                           break;
          case '$': cursor->score += 5;                           break;
          case '+': cursor->length += lengthChange;               break;
          case '-': cursor->length -= lengthChange;               break;
          case char(24): cursor->bottomline -= elevationChange;   break; // Char(24) is an up-arrow
          case char(25): cursor->bottomline += elevationChange;   break; // Char(25) is a down-arrow
          case char(19): cursor->playing = false;                 break; // Char(19) is a !!-sign
          default: Serial.println("ERROR: Unexpected sprite in checkCollision");
        }
      } else {
        if (drop->sprite == '*' or drop->sprite == '$') { // Ensures a loss if point-giving drops are missed
          cursor->playing = false;
        }
      }
      drop->enabled = false;
    }
  }
}

// Ensures that the cursor does not go out of bounds
void ensureBoundaries(struct cursor *cursor) {
  if (cursor->bottomline > SCREEN_HEIGHT - 1) {
    cursor->bottomline = SCREEN_HEIGHT - 1;
  }
}

// Draws and updates positions of drops
void updateDrop(struct drop *drop, struct cursor cursor) {
  display.fillRect(drop->pos[0], drop->pos[1], cWidth, cHeight, 0);
  unsigned int cT = millis();
  if (drop->enabled) {
    drop->pos[1] = (cT - drop->born) * (drop->ySpeed * (1.0 + float(cursor.score) / 20.0)); // Added increasing difficulty
    display.drawChar(drop->pos[0], drop->pos[1], drop->sprite, 1, 0, 1);
  }
}

// Draws and updates position of cursor
void updateCursor(struct cursor *cursor) {
  display.fillRect(0, cursor->bottomline, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  cursor->pos = map(analogRead(potPin), 0, 1023, SCREEN_WIDTH - cursor->length, 0);
  display.drawLine(cursor->pos, cursor->bottomline, cursor->pos + cursor->length, cursor->bottomline, 1);
}

// Draws score in the upper right corner
void drawScore(struct cursor cursor) {
  int score = cursor.score;
  display.fillRect(SCREEN_WIDTH - 5 * cWidth - 1, 0, 5 * cWidth + 1, 2 * cHeight, 1);
  display.setCursor(SCREEN_WIDTH - 5 * cWidth, 1);
  display.setTextColor(0);
  display.setTextSize(1);
  display.print("SCORE");
  display.setCursor(SCREEN_WIDTH - 5 * cWidth, cHeight + 1);
  display.print(cursor.score);
}

// Buzzes using the piezo speaker
void buzz(char mode) {
  if (!mute) {
    switch (mode) {
      case 'H': // Hit
        tone(piezoPin, 440, 100);
        break;
      case 'L': // Loss
        tone(piezoPin, 196, 100);
        delay(150);
        tone(piezoPin, 196, 250);
        break;
      case 'S': // Start
        int tones[3] = {880, 932, 988};
        for (int i = 0; i < 3; i++) {
          tone(piezoPin, tones[i], 100);
          delay(100);
        }
        break;
    }
  }
}

// Main game loop. Returns score.
int catchGame() {
  display.clearDisplay();
  drop drops[maxDrops];
  cursor cursor;
  lastDrop = 0;
  buzz('S');
  while (cursor.playing) {
    spawnDrops(drops, maxDrops);
    dropInterval = initDropInterval / (1.0 + (float(cursor.score) / 15.0)); // Increasing difficulty
    for (int i = 0; i < maxDrops; i++) {
      updateDrop(&drops[i], cursor);
      checkCollision(&drops[i], &cursor);
    }
    updateCursor(&cursor);
    ensureBoundaries(&cursor);
    drawScore(cursor);
    display.display();
  }
  buzz('L');
  return cursor.score;
}
