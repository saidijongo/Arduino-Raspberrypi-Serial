#include <AccelStepper.h>

const int motorInterfaceType = 1;
AccelStepper stepper(motorInterfaceType, 5, 6);

const int rightSensorPin = 2;
const int leftSensorPin = 3;
const int buttonPin = 1;

bool rotatingClockwise = false;
bool rotatingCounterClockwise = false;
bool stopRequested = false;
bool rotating30CW = false;
bool rotating30CCW = false;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

int lastButtonState = HIGH;
int buttonState = HIGH;

void setup() {
  stepper.setMaxSpeed(2000);
  stepper.setAcceleration(1000);

  pinMode(rightSensorPin, INPUT_PULLUP);
  pinMode(leftSensorPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  checkSensorInterrupts();
  checkButton();

  if (rotatingClockwise) {
    rotateClockwise();
  } else if (rotatingCounterClockwise) {
    rotateCounterClockwise();
  } else if (rotating30CW) {
    rotateClockwise(30);
  } else if (rotating30CCW) {
    rotateCounterClockwise(30);
  } else if (stopRequested) {
    stopMotor();
  }

  if (Serial.available() > 0) {
    char command = Serial.read();
    executeCommand(command);
  }
}

void checkSensorInterrupts() {
  if (digitalRead(rightSensorPin) == LOW) {
    rotatingClockwise = true;
    rotatingCounterClockwise = false;
    stopRequested = false;
    rotating30CW = false;
    rotating30CCW = false;
    while (digitalRead(rightSensorPin) == LOW) {
      // Wait until sensor is no longer interrupted
    }
    delay(3000); // Stop for 3 seconds
    rotatingClockwise = false;
    rotatingCounterClockwise = true;
  }

  if (digitalRead(leftSensorPin) == LOW) {
    rotatingClockwise = false;
    rotatingCounterClockwise = true;
    stopRequested = false;
    rotating30CW = false;
    rotating30CCW = false;
    while (digitalRead(leftSensorPin) == LOW) {
      // Wait until sensor is no longer interrupted
    }
    delay(3000); // Stop for 3 seconds
    rotatingCounterClockwise = false;
    rotatingClockwise = true;
  }
}

void checkButton() {
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        rotatingClockwise = false;
        rotatingCounterClockwise = false;
        stopRequested = false;
        rotating30CW = true;
        rotating30CCW = false;
      } else {
        rotating30CW = false;
      }
    }
  }

  lastButtonState = reading;
}

void rotateClockwise() {
  stepper.moveTo(360);
  stepper.run();
}

void rotateCounterClockwise() {
  stepper.moveTo(-360);
  stepper.run();
}

void rotateClockwise(int degrees) {
  stepper.move(degrees);
  stepper.run();
}

void rotateCounterClockwise(int degrees) {
  stepper.move(-degrees);
  stepper.run();
}

void stopMotor() {
  stepper.stop();
  stepper.setCurrentPosition(0);
}

void executeCommand(char command) {
  switch (command) {
    case 'C':
      rotatingClockwise = true;
      rotatingCounterClockwise = false;
      stopRequested = false;
      rotating30CW = false;
      rotating30CCW = false;
      break;
    case 'D':
      rotatingClockwise = false;
      rotatingCounterClockwise = true;
      stopRequested = false;
      rotating30CW = false;
      rotating30CCW = false;
      break;
    case 'S':
      rotatingClockwise = false;
      rotatingCounterClockwise = false;
      stopRequested = true;
      rotating30CW = false;
      rotating30CCW = false;
      break;
    default:
      // Invalid command
      break; 
  }
}
