unsigned int lastDrop;
unsigned int dropInterval = initDropInterval;

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
  drop.born = millis();
  drop.enabled = true;
  return drop;
}

void spawnDrops(struct drop drops[], int dropCount) {
  for (int i = 0; i < dropCount; i++) {
    unsigned int cT = millis();
    if (!drops[i].enabled and cT - lastDrop > dropInterval) {
      drops[i] = genDrop();
      lastDrop = millis();
    }
  }
}

void checkCollision(struct drop *drop, struct cursor *cursor) {
  if (drop->pos[1] > cursor->bottomline) {
    if (drop->enabled) {
      if (cursor->pos - captureBuffer <= drop->pos[0] and drop->pos[0] <= cursor->pos + cursor->length + captureBuffer) {
        buzz('H');
        switch (drop->sprite) {
          case '*': cursor->score += 1;                           break;
          case '$': cursor->score += 5;                           break;
          case '+': cursor->length += lengthChange;               break;
          case '-': cursor->length -= lengthChange;               break;
          case char(24): cursor->bottomline -= elevationChange;   break;
          case char(25): cursor->bottomline += elevationChange;   break;
          case char(19): cursor->playing = false;                 break;
          default: Serial.println("ERROR: Unexpected sprite in checkCollision");
        }
      } else {
        if (drop->sprite == '*' or drop->sprite == '$') {
          cursor->playing = false;
        }
      }
      drop->enabled = false;
    }
  }
}

void ensureBoundaries(struct cursor *cursor) {
  if (cursor->bottomline > SCREEN_HEIGHT - 1) {
    cursor->bottomline = SCREEN_HEIGHT - 1;
  }
}

void updateDrop(struct drop *drop, struct cursor cursor) {
  display.fillRect(drop->pos[0], drop->pos[1], cWidth, cHeight, 0);
  unsigned int cT = millis();
  if (drop->enabled) {
    drop->pos[1] = (cT - drop->born) * (drop->ySpeed * (1.0 + float(cursor.score) / 20.0));
    display.drawChar(drop->pos[0], drop->pos[1], drop->sprite, 1, 0, 1);
  }
}

void updateCursor(struct cursor *cursor) {
  display.fillRect(0, cursor->bottomline, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  cursor->pos = map(analogRead(potPin), 0, 1023, SCREEN_WIDTH - cursor->length, 0);
  display.drawLine(cursor->pos, cursor->bottomline, cursor->pos + cursor->length, cursor->bottomline, 1);
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
}

void buzz(char mode) {
  if (!mute) {
    switch (mode) {
      case 'H':
        tone(piezoPin, 440, 100);
        break;
      case 'L':
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

int catchGame() {
  display.clearDisplay();
  drop drops[maxDrops];
  cursor cursor;
  lastDrop = 0;
  while (cursor.playing) {
    spawnDrops(drops, maxDrops);
    dropInterval = initDropInterval / (1.0 + (float(cursor.score) / 15.0));
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
