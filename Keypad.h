int CheckButtons(int keypadPin)
{
  int buttonsPinValue = analogRead(keypadPin);
  int pressedButton = 0;
//    Serial.print("button pin value ");
//    Serial.println(buttonsPinValue);
  if (buttonsPinValue > 1023 - 30) {
    pressedButton = 1;
  } else if (buttonsPinValue > 930 - 30) {
    pressedButton = 2;
  } else if (buttonsPinValue > 850 - 30) {
    pressedButton = 3;
  } else if (buttonsPinValue > 790 - 20) {
    pressedButton = 4;
  } else if (buttonsPinValue > 730 - 20) {
    pressedButton = 5;
  } else if (buttonsPinValue > 680 - 20) {
    pressedButton = 6;
  } else if (buttonsPinValue > 640 - 20) {
    pressedButton = 7;
  } else if (buttonsPinValue > 600 - 20) {
    pressedButton = 8;
  } else if (buttonsPinValue > 570 - 20) {
    pressedButton = 9;
  } else if (buttonsPinValue > 540 - 20) {
    pressedButton = 10;
  } else if (buttonsPinValue > 510 - 10) {
    pressedButton = 11;
  } else if (buttonsPinValue > 490 - 10) {
    pressedButton = 12;
  } else if (buttonsPinValue > 100) {
//    Serial.print("button pin value ");
//    Serial.println(buttonsPinValue);
//    pressedButton = 999;
  }
  return pressedButton;
}
