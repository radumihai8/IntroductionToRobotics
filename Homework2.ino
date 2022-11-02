const int buttonPin = 2;
const int buzzerPin = 3;

const int pedGreenLedPin = 4;
const int pedRedLedPin = 5;

const int vehGreenLedPin = 6;
const int vehYellowLedPin = 7;
const int vehRedLedPin = 8;
 
int state = 1;

const int firstStateDuration = 8000;
const int secondStateDuration = 3000;
const int thirdStateDuration = 8000;
const int fourthStateDuration = 4000;

const int buzzFrequency = 600;
const int slowBuzzInterval = 1000;
const int fastBuzzInterval = 500;
const int buzzDuration = 250;

const int blinkInterval = 500;
int blinkLedValue = LOW;
 
byte reading = LOW;
byte lastReading = LOW;
byte buttonState = LOW;
byte buttonPressed = LOW;

unsigned long lightTimer = 0; 
unsigned long lastBlink = 0;
unsigned long lastBuzz = 0;
unsigned long lastDebounceTime = 0;
unsigned int debounceDelay = 50;

 
void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pedGreenLedPin, OUTPUT);
  pinMode(pedRedLedPin, OUTPUT);
  pinMode(vehGreenLedPin, OUTPUT);
  pinMode(vehYellowLedPin, OUTPUT);
  pinMode(vehRedLedPin, OUTPUT);
}
 
void loop() {
  // button reading 
  reading = digitalRead(buttonPin);
 
  if(reading != lastReading) {
      lastDebounceTime = millis();
  }
 
  if(millis() - lastDebounceTime > debounceDelay) {
    if(reading != buttonState) {
      buttonState = reading;
      if(buttonState == LOW) {
        // Check if the state is 1 and button has not been press so pressing the button more than once doesn't reset the timer
        if(state == 1 && !buttonPressed)
          buttonPressed = HIGH;
          lightTimer = millis();
      }
    }
 
  }
 
  lastReading = reading;

  // state handling
  if (state == 1 && buttonPressed && millis() - lightTimer > firstStateDuration) {
    buttonPressed = 0;
    state = 2;
    lightTimer = millis();
  }
  else if(state == 2 && millis() - lightTimer > secondStateDuration){
    state = 3;
    lightTimer = millis();
  }
  else if(state == 3 && millis() - lightTimer > thirdStateDuration){
    state = 4;
    lightTimer = millis();
  }
  else if(state == 4 && millis() - lightTimer > fourthStateDuration){
    state = 1;
    lightTimer = millis();
  }

  // state handling
  if(state==1){
    digitalWrite(pedGreenLedPin, LOW);
    digitalWrite(pedRedLedPin, HIGH);

    digitalWrite(vehGreenLedPin, HIGH);
    digitalWrite(vehYellowLedPin, LOW);
    digitalWrite(vehRedLedPin, LOW);
  }
  else if(state==2){
    digitalWrite(pedGreenLedPin, LOW);
    digitalWrite(pedRedLedPin, HIGH);

    digitalWrite(vehGreenLedPin, LOW);
    digitalWrite(vehYellowLedPin, HIGH);
    digitalWrite(vehRedLedPin, LOW);
  }
  else if(state==3){
    digitalWrite(pedGreenLedPin, HIGH);
    digitalWrite(pedRedLedPin, LOW);

    digitalWrite(vehGreenLedPin, LOW);
    digitalWrite(vehYellowLedPin, LOW);
    digitalWrite(vehRedLedPin, HIGH);

    if (millis() - lastBuzz >= slowBuzzInterval) {
      lastBuzz = millis();
      tone(buzzerPin, buzzFrequency, buzzDuration);
    }

  }
  else if(state==4){
    digitalWrite(pedRedLedPin, LOW);

    if (millis() - lastBlink > blinkInterval) {
      blinkLedValue = !blinkLedValue;
      lastBlink = millis();
    }

    digitalWrite(pedGreenLedPin, blinkLedValue);
    
    digitalWrite(vehRedLedPin, HIGH);
    digitalWrite(vehYellowLedPin, LOW);
    digitalWrite(vehGreenLedPin, LOW);

    if (millis() - lastBuzz >= fastBuzzInterval) {
      lastBuzz = millis();
      tone(buzzerPin, buzzFrequency, buzzDuration);
    }
  }

}
 