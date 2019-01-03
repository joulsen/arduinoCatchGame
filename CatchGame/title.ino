// Draw title screen.
int drawTitle() {
  int last_state = -1;
  windowStart = millis();
  unsigned int cT = millis();
  display.clearDisplay();
  display.setTextSize(2); // Double text size.
  display.setTextColor(1);
  display.cp437(true); // Use typesetting code page 437 as listed in Adafruit GFX library.
  display.setCursor((SCREEN_WIDTH - 10 * 2 * cWidth) / 2, 10);
  display.print("Catch Game");
  //display.display();
  while (digitalRead(buttonPin) == LOW or cT - windowStart < minReturnTime) {
    cT = millis();
    if (analogRead(potPin) / 512 != last_state) {
      last_state = analogRead(potPin) / 512;
      display.fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
      display.setCursor((SCREEN_WIDTH - 8 * 2 * cWidth) / 2, 40);
      if (last_state == 1) {
        display.print("  PLAY ");
        display.write(16);
      } else if (last_state == 0) {
        display.write(17);
        display.print(" HELP");
      }
      display.display();
    }
  }
  return last_state;
}
