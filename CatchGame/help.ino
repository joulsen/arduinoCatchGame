void drawHelp() {
  windowStart = millis();
  T0 = millis();
  display.clearDisplay();
  display.setCursor(1, 1);
  display.setTextSize(1);
  display.setTextColor(1);
  display.println("* $: +1/+5 Point(s)");
  display.println("+ -: Cursor length");
  display.print(char(24));
  display.print(' ');
  display.print(char(25));
  display.println(": Cursor elevation");
  display.print(char(19));
  display.println("  : End game");
  // Display return message
  display.setCursor(1, SCREEN_HEIGHT - cHeight);
  display.print(char(27));
  display.print(" Press button");
  display.display();
  while (digitalRead(buttonPin) == LOW or T0 - windowStart < minReturnTime) {
    T0 = millis();
    Serial.print(T0);
    Serial.print(',');
    Serial.println(windowStart);
    delay(100);
  }
}
