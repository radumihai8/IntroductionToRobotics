// declare all the joystick pins
const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

// declare all the segments pins
const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

bool commonAnode = false; 

const int segSize = 8;

byte swState = LOW;
byte lastSwState = HIGH;
unsigned long swPressTime = 0;
const int longSwPressThreshold = 1000;

int xValue = 0;
int yValue = 0;

bool joyMoved = false;
int minThreshold = 400;
int maxThreshold = 600;

int currentPos = segSize - 1;

const int blinkTime = 400;
bool currentPosState = LOW;
unsigned long lastBlinkTime = 0;

int currentState = 1;

byte activeLeds[segSize] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

int segments[segSize] = { 
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int movementMatrix[10][10] = {
  // UP, DOWN, LEFT, RIGHT
  {-1, 6, 5, 1}, //0 - a
  {0, 6, 5, -1}, //1 - b
  {6, 3, 4, 7}, //2 - c
  {6, -1, 4, 2}, //3 - d
  {6, 3, -1, 2}, //4 - e
  {0, 6, -1, 1}, //5 - f
  {0, 3, -1, -1}, //6 - g
  {-1, -1, 2, -1}, //7 - dp
};

void setup() {
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  if(currentState == 1){
    int nextPos = -1;
    if (joyMovedDown()) {
      nextPos = movementMatrix[currentPos][1];
    }
    if (joyMovedUp()) {
      nextPos = movementMatrix[currentPos][0];
    }
    if (joyMovedLeft()) {
      nextPos = movementMatrix[currentPos][2];
    }
    if (joyMovedRight()) {
      nextPos = movementMatrix[currentPos][3];
    }

    if (nextPos != -1){
      currentPos = nextPos;  
    }

    joyMoved = true;
  }
  else{
    if (joyMovedUp() || joyMovedDown()) {
      activeLeds[currentPos] = !activeLeds[currentPos];
      joyMoved = true;
    }
  }

  if (joyIsNeutral()) {
    joyMoved = false;
  }

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

  refreshDisplay();
  delay(1);
}

void resetBoard(){
  for (int i = 0; i < segSize; i++){
    activeLeds[i] = LOW;
  }
  currentState = 1;
  currentPos = segSize - 1;
}

void switchState(){
  if(currentState == 1)
    currentState = 2;
  else
    currentState = 1;
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

void refreshDisplay() {
  if(currentState == 1){
    //Blink the current position
    if(millis() - lastBlinkTime > blinkTime){
      currentPosState = !currentPosState;
      lastBlinkTime = millis();
    }
    digitalWrite(segments[currentPos], currentPosState ^ commonAnode);
  }
  
  for (int i = 0; i < segSize; i++) {
    if(i != currentPos || currentState == 2)
      digitalWrite(segments[i], activeLeds[i] ^ commonAnode);
  }
}


