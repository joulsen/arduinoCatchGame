int drawTitle() {
  int last_state = -1;
  int windowStart = millis();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(1);
  display.cp437(true); // Use typesetting code page 437.
  display.setCursor((SCREEN_WIDTH - 10 * 2 * cWidth) / 2, 10);
  display.print("Catch Game");
  display.display();
  while (digitalRead(buttonPin) == LOW or windowStart + minReturnTime > millis()) {
    if (analogRead(potPin) / 342 != last_state) {
      last_state = analogRead(potPin) / 342;
      display.fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
      display.setCursor((SCREEN_WIDTH - 8 * 2 * cWidth) / 2, 40);
      if (last_state == 2) {
        display.print("  PLAY ");
        display.write(16);
      } else if (last_state == 1) {
        display.write(17);
        display.print(" OPTI ");
        display.write(16);
      } else if (last_state == 0) {
        display.write(17);
        display.print(" HELP  ");
      }
      display.display();
    }
  }
  return last_state;
}
