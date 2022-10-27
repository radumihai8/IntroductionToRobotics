const int redLedPin = 9;
const int greenLedPin = 10;
const int blueLedPin = 11;

const int redPotPin = A0;
const int greenPotPin = A1;
const int bluePotPin = A2;

const long minPotValue = 0;
const long maxPotValue = 1023;

const long minLedValue = 0;
const long maxLedValue = 255;

const byte anodComun = 0;
 
int redPotValue, greenPotValue, bluePotValue;

int redLedValue, greenLedValue, blueLedValue;
 
void setup() {
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

  pinMode(redPotPin, INPUT);
  pinMode(greenPotPin, INPUT);
  pinMode(bluePotPin, INPUT);
}
 
void loop() {
  redPotValue = analogRead(redPotPin);
  greenPotValue = analogRead(greenPotPin);
  bluePotValue = analogRead(bluePotPin);

  redLedValue = map(redPotValue, minPotValue, maxPotValue, minLedValue, maxLedValue);
  greenLedValue = map(greenPotValue, minPotValue, maxPotValue, minLedValue, maxLedValue);
  blueLedValue = map(bluePotValue, minPotValue, maxPotValue, minLedValue, maxLedValue);
 
  setColor(redLedValue, greenLedValue, blueLedValue);

  delay(10);
}
 
void setColor(int redValue, int greenValue, int blueValue){
  if(anodComun) {
    redValue = maxLedValue - redValue;
    greenValue = maxLedValue - greenValue;
    blueValue = maxLedValue - blueValue;
  }
  analogWrite(redLedPin, redValue);
  analogWrite(greenLedPin, greenValue);
  analogWrite(blueLedPin, blueValue);
}