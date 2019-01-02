void showScore(int score) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(1);
  display.setCursor((SCREEN_WIDTH - 5 * 2 * cWidth) / 2, 10);
  display.print("SCORE");
  int scoreDigits = 0;
  int score_copy = score;
  while (score_copy != 0) {
    score_copy = score_copy / 10;
    scoreDigits++;
  }
  display.setCursor((SCREEN_WIDTH - scoreDigits * 2 * cWidth) / 2, 15 + 2*cHeight);
  display.print(score);
  display.display();
  delay(1000);
  return;
}
