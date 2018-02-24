#include <SoftwareSerial.h>
#include "DFMiniMp3.h"
#include "MP3Notify.h"
#include "Keypad.h"
#include "Context.h"

// Encoder Pin C = GND
int encoderPinB = 3; // D2 - IRQ0
int encoderPinA = 2; // D3 - IRQ1
int encoderSwitchPin = 4; // D4 //push button switch

//int ledRPin = 9; // D9 PWM
//int ledGPin = 6; // D6 PWM
int ledBPin = 5; // D5 PWM

int mp3RXPin = 7; // D7
int mp3TXPin = 8; // D8

int keypadPin = 14; // A0


// Ecoder Vars
volatile int lastEncoded = 0;
volatile long encoderValue = 0;
long lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;

// App Vars
Context context;
int lastEncoderValue = 0;
int lastKeypadPressed = 0;
boolean lastEncoderSwitchPressed = false;
int volume = 20; // 0 - 30


SoftwareSerial mp3Serial(mp3RXPin, mp3TXPin);
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(mp3Serial);

void setup() {
  Serial.begin(115200);
  Serial.println(F("initializing..."));

  // First, set all the pins

  /// Encoder Pins
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  digitalWrite(encoderPinA, HIGH); //turn pullup resistor on
  digitalWrite(encoderPinB, HIGH); //turn pullup resistor on
  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);

  pinMode(encoderSwitchPin, INPUT);
  digitalWrite(encoderSwitchPin, HIGH); //turn pullup resistor on

  /// LED Pins
//  pinMode(ledRPin, OUTPUT);
//  pinMode(ledGPin, OUTPUT);
  pinMode(ledBPin, OUTPUT);

  /// Keypad Pin
  pinMode(keypadPin, INPUT);

  /// MP3 Pins
  pinMode(mp3TXPin, OUTPUT);
  pinMode(mp3RXPin, INPUT);
  setVolume(0);
  Serial.println(F("Setup done..."));

  context.mp3 = &mp3;
  mp3.begin(&context);
  mp3.setEq(DfMp3_Eq_Pop);
  mp3.enableDAC();
  setVolume(0);


  Serial.println(F("Scanning track counts"));
  Serial.print(F("Total track count: "));
  Serial.println(mp3.getTotalTrackCount());
  
  for (int i = 1; i <= 12; i++) {
    // Folders are 1-indexed for `getFolderTrackCount`
    context.tracksInFolders[i] = mp3.getFolderTrackCount(i);
  }

  for (int i = 1; i <= 12; i++) {
    Serial.print(F("Folder "));
    Serial.print(i);
    Serial.print(F(" has: "));
    Serial.println(context.tracksInFolders[i]); //read fill counts in folder SD:/i
  }

  // light it up!
  setVolume(volume);
}

void loop() {
  //Do stuff here
  boolean encoderSwitchPressed = digitalRead(encoderSwitchPin);
  if (encoderSwitchPressed) {
    //button is not being pushed
    if (!lastEncoderSwitchPressed) {
      Serial.println(F("Pushed"));
      togglePlayPause(&context);
    }
  } else {
    //button is being pushed
  }
  lastEncoderSwitchPressed = encoderSwitchPressed;

  updateVolume();

  int keypadPressed = CheckButtons(keypadPin);

  if (keypadPressed != lastKeypadPressed) {
    lastKeypadPressed = keypadPressed;

    if (keypadPressed > 0) {
      Serial.print(F("Taste: "));
      Serial.println(keypadPressed);
      int folder = folderForKeypadButton(keypadPressed);
      int track = 1;
      if (folder == context.currentFolder) {
        playNextTrack(&context);
      } else {
        playFolderAndTrack(folder, track, &context);
      }
    }
  }

  waitMilliseconds(50);
}

int folderForKeypadButton(int button) {
  switch(button) {
    case 1:  return 4;
    case 2:  return 8;
    case 3:  return 12;
    case 4:  return 3;
    case 5:  return 7;
    case 6:  return 11;
    case 7:  return 2;
    case 8:  return 6;
    case 9:  return 10;
    case 10: return 1;
    case 11: return 5;
    case 12: return 9;
  }
}


void waitMilliseconds(uint16_t msWait) {
  uint32_t start = millis();
  while ((millis() - start) < msWait) {
    // calling mp3.loop() periodically allows for notifications
    // to be handled without interrupts
    mp3.loop();

    delay(1);
  }
}

void setVolume(int volume) {
    mp3.setVolume(volume);
    Serial.print(F("Setting volume to "));
    Serial.print(volume);
    Serial.print(F(". Successfully set to  "));
    Serial.println(mp3.getVolume());
    setLEDColor(255-PWMValueForVolumne(volume), PWMValueForVolumne(volume), PWMValueForVolumne(volume));
}

void updateVolume() {
  if (lastEncoderValue != encoderValue) {
    Serial.println(lastEncoderValue);
    int encoderDiff = encoderValue - lastEncoderValue;
    lastEncoderValue = encoderValue;
    volume += encoderDiff/3;
    volume = max(0, min(30, volume));

    setVolume(volume);
  }
}

void setLEDColor(int red, int green, int blue) {
  // Adjust for common anode
  red = 255 - max(0, min(255, red));
  green = 255 - max(0, min(255, green));
  blue = 255 - max(0, min(255, blue));

//  analogWrite(ledRPin, red);
//  analogWrite(ledGPin, green);
  analogWrite(ledBPin, blue);
}

int PWMValueForVolumne(int input) {
if (input <=   0 ) return  0 ;
if (input ==  1 ) return  1 ;
if (input ==  2 ) return  2 ;
if (input ==  3 ) return  3 ;
if (input ==  4 ) return  4 ;
if (input ==  5 ) return  5 ;
if (input ==  6 ) return  6 ;
if (input ==  7 ) return  7 ;
if (input ==  8 ) return  8 ;
if (input ==  9 ) return  9 ;
if (input ==  10  ) return  10  ;
if (input ==  11  ) return  13  ;
if (input ==  12  ) return  17  ;
if (input ==  13  ) return  21  ;
if (input ==  14  ) return  26  ;
if (input ==  15  ) return  32  ;
if (input ==  16  ) return  39  ;
if (input ==  17  ) return  47  ;
if (input ==  18  ) return  56  ;
if (input ==  19  ) return  65  ;
if (input ==  20  ) return  76  ;
if (input ==  21  ) return  88  ;
if (input ==  22  ) return  101 ;
if (input ==  23  ) return  115 ;
if (input ==  24  ) return  131 ;
if (input ==  25  ) return  148 ;
if (input ==  26  ) return  166 ;
if (input ==  27  ) return  186 ;
if (input ==  28  ) return  208 ;
if (input ==  29  ) return  231 ;
if (input >=  30  ) return  255 ;
}


void updateEncoder() {
  int MSB = digitalRead(encoderPinA); //MSB = most significant bit
  int LSB = digitalRead(encoderPinB); //LSB = least significant bit

  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded; //store this value for next time
}
