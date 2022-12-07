#include <LiquidCrystal.h>
#include <EEPROM.h>
 
const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

int xValue = 0;
int yValue = 0;

byte swState = LOW;
byte lastSwState = HIGH;

bool joyMoved = false;
int minThreshold = 400;
int maxThreshold = 600;

const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
 
const byte lcdContrastPin = 3;
 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 
byte lcdContrast = 120;
int currentMenuOption = 1;
int currentMenuSubOption = 1;

// 0 - greeting, 1 - menu, 2 - submenu, 3 - game, 4 - ending
int currentState = 0;

int startTime = 0;

int currentDifficulty = 1;
int currentBrightness = 100;
bool currentSounds = 1;

int contrastAddress = 0;
int difficultyAddress = 1;
int brightnessAddress = 2;
int soundsAddress = 3;
int nameAddress = 4;
int highScoreAddress = 5;

// TODO: retrieve from eeprom
int highScore = 10000;

int currentScore = 0;
 
void setup() {
  Serial.begin(9600);
  pinMode(lcdContrastPin, OUTPUT);
  pinMode(pinSW, INPUT_PULLUP);
 
  lcd.begin(16, 2);
  lcd.print("Hello!");
  //loadSettings();
  analogWrite(lcdContrastPin, lcdContrast);
  startTime = millis();
}
 
void loop() {

  if(millis()-startTime > 2000 && currentState == 0 ){
    currentState = 1;
    currentMenuOption = 1;
    changeMenuOption();
  }

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  if(currentState == 1){

    int oldMenuState = currentMenuOption;
    
    if(joyMovedDown()){
      currentMenuOption += 1;
      joyMoved = true;
    }
    if(joyMovedUp()){
      currentMenuOption -= 1;
      joyMoved = true;
    }

    if(currentMenuOption > 5){
      currentMenuOption = 1;
    }
    if(currentMenuOption < 1){
      currentMenuOption = 5;
    }

    if(joyMoved && oldMenuState != currentMenuOption){
      changeMenuOption();
    }

    if (joyIsNeutral()) {
      joyMoved = false;
    }

    swState = digitalRead(pinSW);
    if (swState != lastSwState) {
      if (swState == LOW) {
        changeMenuState();
      }

    }

  lastSwState = swState;
  }
  else if(currentState == 2){
    int oldMenuSubState = currentMenuSubOption;
    
    if(joyMovedDown()){
      currentMenuSubOption += 1;
      joyMoved = true;
    }
    if(joyMovedUp()){
      currentMenuSubOption -= 1;
      joyMoved = true;
    }
    if(joyMovedLeft()){
      joyMoved = true;
      decreaseSetting();
    }
      
    if(joyMovedRight()){
      joyMoved = true;
      increaseSetting();
    }
      

    if(currentMenuSubOption > 6){
      currentMenuSubOption = 1;
    }
    if(currentMenuSubOption < 1){
      currentMenuSubOption = 6;
    }

    if(joyMoved && oldMenuSubState != currentMenuSubOption){
      changeMenuSubOption();
    }

    if (joyIsNeutral()) {
      joyMoved = false;
    }

    swState = digitalRead(pinSW);
    if (swState != lastSwState) {
      if (swState == LOW) {
        changeMenuState();
      }
    }

    lastSwState = swState;
  }
  else if(currentState == 4){
    swState = digitalRead(pinSW);
    if (swState != lastSwState) {
      if (swState == LOW) {
        Serial.println("Resetting");
        currentState = 1;
        currentMenuOption = 1;
        changeMenuOption();
      }
    }
    lastSwState = swState;
  }
  
 
}

void increaseSetting(){
  switch(currentMenuSubOption){
      case 1:
        break;
      case 2:
        currentDifficulty += 1;
        break;
      case 3:
        lcdContrast += 10;
        analogWrite(lcdContrastPin, lcdContrast);
        break;
      case 4:
        currentBrightness += 10;
        break;
      case 5:
        currentSounds = !currentSounds;
        break;
      case 6:
        break;
  }
  changeMenuSubOption();
}

void decreaseSetting(){
  switch(currentMenuSubOption){
      case 1:
        break;
      case 2:
        currentDifficulty -= 1;
        break;
      case 3:
        lcdContrast -= 10;
        analogWrite(lcdContrastPin, lcdContrast);
        break;
      case 4:
        currentBrightness -= 10;
        break;
      case 5:
        currentSounds = !currentSounds;
        break;
      case 6:
        break;
  }
  changeMenuSubOption();
}

void changeMenuState(){
  if(currentState == 1){
    switch(currentMenuOption){
      case 1:
        startGame();
        break;
      case 2:
        break;
      case 3:
        currentMenuSubOption = 1;
        currentState = 2;
        changeMenuSubOption();
    }
  }
  if(currentState == 2){
    switch(currentMenuSubOption){
      case 1:
        break;
      case 2:
        break;
      case 3:
        break;
      case 4:
        break;
      case 5:
        break;
      case 6:
        saveSettings();
        currentState = 1;
        changeMenuOption();
        break;

    }
  }
}

void loadSettings(){
  lcdContrast = EEPROM.read(contrastAddress);
  //currentDifficulty = EEPROM.read(difficultyAddress);
  //currentBrightness = EEPROM.read(brightnessAddress);
  //currentSounds = EEPROM.read(soundsAddress);
}

void saveSettings(){
  Serial.println("Settings saved");
  EEPROM.update(contrastAddress, lcdContrast);
  //EEPROM.update(difficultyAddress, currentDifficulty);
  //EEPROM.update(brightnessAddress, currentBrightness);
  //EEPROM.update(soundsAddress, currentSounds);
}

void startGame(){
  lcd.clear();
  lcd.print("Pts: 0 ");
  lcd.print("L: 3");
  currentScore = 1904;
  delay(5000);
  endGame();
}

void endGame(){
  currentState = 4;
  lcd.clear();
  lcd.print("Your score: ");
  lcd.print(currentScore);
  delay(5000);
  lcd.clear();
  lcd.print("Didn't beat HS");
  lcd.setCursor(0, 1);
  lcd.print("Click to retry");
  lcd.setCursor(0, 0);
}

void changeMenuOption(){
  Serial.println("changing state");
  lcd.clear();
  switch (currentMenuOption) {
    case 1:
      Serial.println("changing state1111");
      lcd.print("Start game");
      break;
    case 2:
      lcd.print("Highscore: ");
      lcd.setCursor(0,1);
      lcd.print(highScore);
      break;
    case 3:
      lcd.print("Settings");
      break;
    case 4:
      lcd.print("About");
      break;
    case 5:
      lcd.print("How to play");
      break;
    default:
      lcd.print("Default");
      break;
  }
}

void changeMenuSubOption(){
  Serial.println("changing state");
  lcd.clear();
  switch(currentMenuOption){
    case 3:
      switch (currentMenuSubOption) {
        case 1:
          Serial.println("change menu suboption 1");
          lcd.print("Enter Name");
          break;
        case 2:
          Serial.println("change menu suboption 2");
          lcd.print("Difficulty ");
          lcd.print(currentDifficulty);
          break;
        case 3:
          Serial.println("change menu suboption 3");
          lcd.print("Contrast ");
          lcd.print(lcdContrast);
          break;
        case 4:
          Serial.println("change menu suboption 4");
          lcd.print("Brightness ");
          lcd.print(currentBrightness);
          break;
        case 5:
          Serial.println("change menu suboption 5");
          lcd.print("Sounds ");
          lcd.print(currentSounds);
          break;
        case 6:
          Serial.println("change menu suboption 6");
          lcd.print("Back to main menu");
          break;  
        default:
          lcd.print("Default");
          break;
      }
  }
}


bool joyIsNeutral(){
  return xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold;
}

bool joyMovedDown(){
  if (xValue < minThreshold && joyMoved == false)
    return true;
  return false;
}

bool joyMovedUp(){
  if (xValue > maxThreshold && joyMoved == false)
    return true;
  return false;
}

bool joyMovedLeft(){
  if (yValue < minThreshold && joyMoved == false)
    return true;
  return false;
}

bool joyMovedRight(){
  if (yValue > maxThreshold && joyMoved == false)
    return true;
  return false;
}
