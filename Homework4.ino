// declare all the joystick pins
const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

//DS= [D]ata [S]torage - data
//STCP= [ST]orage [C]lock [P]in latch
//SHCP= [SH]ift register [C]lock [P]in clock

const int latchPin = 11; // STCP to 12 on Shift Register
const int clockPin = 10; // SHCP to 11 on Shift Register
const int dataPin = 12; // DS to 14 on Shift Register

const int displayCount = 4;

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

bool joyMoved = false;
int xValue = 0;
int yValue = 0;
const int minThreshold = 400;
const int maxThreshold = 600;

byte swState = LOW;
byte lastSwState = HIGH;
unsigned long swPressTime = 0;
const int longSwPressThreshold = 1000;

int currentDisplayNumbers[displayCount] = {0, 0, 0, 0};

int currentState = 1;
int currentDisplayIndex = 0;

int displayDigits[] = {
  segD1, segD2, segD3, segD4
};

const int encodingsNumber = 16;

int byteEncodings[encodingsNumber] = {
//A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};

const int blinkTime = 400;
bool lastDPState = 1;
unsigned long lastBlinkTime = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  pinMode(pinSW, INPUT_PULLUP);

}

void loop(){
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  //read left and right
  if(currentState == 1){
    if (joyMovedLeft()) {
      currentDisplayIndex++;
      if(currentDisplayIndex > displayCount - 1)
        currentDisplayIndex = 0;
      joyMoved = true;
    }
    else if (joyMovedRight()) {
      currentDisplayIndex--;
      if(currentDisplayIndex < 0)
        currentDisplayIndex = displayCount - 1;
      joyMoved = true;
    }
  }
  //read up and down
  if(currentState == 2){
    if (joyMovedUp()) {
      currentDisplayNumbers[currentDisplayIndex]++;
      if(currentDisplayNumbers[currentDisplayIndex] > encodingsNumber)
        currentDisplayNumbers[currentDisplayIndex] = 0;
      joyMoved = true;
    }
    else if (joyMovedDown()) {
      currentDisplayNumbers[currentDisplayIndex]--;
      if(currentDisplayNumbers[currentDisplayIndex] < 0)
        currentDisplayNumbers[currentDisplayIndex] = encodingsNumber;
      joyMoved = true;
    }
  }
  if (joyIsNeutral()) {
    joyMoved = false;
  }

  //read sw button
  swState = digitalRead(pinSW);
  if (swState != lastSwState) {
    if (swState == LOW) {
      swPressTime = millis();
    }
    else{
      if(millis() - swPressTime < longSwPressThreshold){
        switchState();
      }
      else{
        resetBoard();
      }
    }
  }

  lastSwState = swState;

  writeNumber();
  delay(1);
}

void resetBoard(){
  for (int i = 0; i < displayCount; i++){
    currentDisplayNumbers[i] = 0;
  }
  currentState = 1;
  currentDisplayIndex = 0;
}

void switchState(){
  if(currentState == 1)
    currentState = 2;
  else
    currentState = 1;
}

void activateDisplay(int digit) {
  for(int i=0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[digit], LOW);
}

void writeReg(int encoding) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, encoding);
  digitalWrite(latchPin, HIGH);
}

void writeNumber() {
  int displayIndex = 0;
  for (auto displayNumber : currentDisplayNumbers) {
    byte encoding = byteEncodings[displayNumber];

    // If the current position matches the global position and currentState is 1 blink DP (last bit)
    if (displayIndex == currentDisplayIndex) {
      if(millis() - lastBlinkTime > blinkTime && currentState == 1){
        lastDPState = !lastDPState;
        lastBlinkTime = millis();
      }
      if(lastDPState || currentState == 2){
        encoding = encoding + 1;
      }
    }

    writeReg(encoding);
    activateDisplay(displayIndex);
    displayIndex++;
    delay(5);
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