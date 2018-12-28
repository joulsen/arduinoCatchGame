void drawHelp() {
  int windowStart = millis();
  display.clearDisplay();
  display.setCursor(1, 1);
  display.setTextSize(1);
  display.setTextColor(1);
  printTrimmed("Catch falling objects in order to increase your score!", 21, 0, 5);
  display.println("*: +1 Point");
  display.print(char(29));
  display.println(": Widens cursor");
  display.print(char(19));
  display.println(": Ends game");
  // Display return message
  display.setCursor(1, SCREEN_HEIGHT - cHeight);
  display.print(char(27));
  display.print(" Press button");
  display.display();
  while (digitalRead(buttonPin) == LOW or windowStart + minReturnTime > millis()) { }
}

void printTrimmed(String string, int linewidth, int fromline, int toline) {
  if (string.length() > linewidth) {
    int lineCount = (string.length() / linewidth);
    if (string.length() % linewidth > 0) {
      lineCount += 1;
    }
    for (int i = 0; i < lineCount; i++) {
      if (fromline <= i and i <= toline) {
        String sub = string.substring(i * linewidth, (i + 1) * linewidth);
        sub.trim();
        display.println(sub);
      }
    }
  } else {
    display.println(string);
  }
  display.display();
}
