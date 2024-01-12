#include <Stepper.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int stepperPin1 = 2;
const int stepperPin2 = 3;
const int speakerPin = 8;


LiquidCrystal_I2C lcd(0x27, 16, 2);

int currentFloor = 1;
const int maxFloors = 3;
const int stepsPerFloor = 5;

const int floor3Button = 7;
const int floor2Button = 6;
const int floor1Button = 5;

//Älä muuta näitä
const int stepsPerRevolution = 100;  
Stepper myStepper(stepsPerRevolution, stepperPin1, stepperPin2);

const float revolutions = 0.5;

void bling() {
  tone(speakerPin, 294, 1000 / 4);
  delay(1000 / 4 * 1.30);
  noTone(speakerPin);
  tone(speakerPin, 392, 1000 / 4);
  delay(1000 / 4 * 1.30);
  noTone(speakerPin);
}

void display(String row1, String row2) {
  if (row1 != "") {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(row1);
  }
  
  lcd.setCursor(0, 1);
  lcd.print(row2);
}

void displayFloor(int floorNumber) {
  display("Current floor:", String(floorNumber));
}

void displayFloorProgress(int floorNumber, int targetFloor, int progress) {
  String progressBar = String(floorNumber);

  for (int i = 0; i < 10; i++) {
    if (i < progress) {
      progressBar += ">"; 
    } else {
      progressBar += " "; 
    }
  }
  
  display("", progressBar + String(targetFloor));
}

void goToFloor(int targetFloor) {
  int revolutions = (currentFloor - targetFloor) * stepsPerFloor;

  display("Going to floor:", String(currentFloor));
  for (int i = 0; i <= 10; i += 2) { 
    displayFloorProgress(currentFloor, targetFloor, i);
    myStepper.step(160 * revolutions);
  }
  
  currentFloor = targetFloor;
  displayFloor(currentFloor);
  
  bling();
}

void setup() {
  Serial.begin(9600);
  
  pinMode(floor3Button, INPUT);
  pinMode(floor2Button, INPUT);
  pinMode(floor1Button, INPUT);
  digitalWrite(floor3Button, HIGH);
  digitalWrite(floor2Button, HIGH);
  digitalWrite(floor1Button, HIGH);
  
  myStepper.setSpeed(500);    
  lcd.begin();

  displayFloor(currentFloor);
}

void loop() {
  if (digitalRead(floor3Button) == LOW && currentFloor != 3) {
      goToFloor(3);
  } else if (digitalRead(floor2Button) == LOW  && currentFloor != 2) {
    goToFloor(2);
  } else if (digitalRead(floor1Button) == LOW  && currentFloor != 1) {
    goToFloor(1);
  }
  

  //Serial.println(String(digitalRead(floor1Button)) + String(digitalRead(floor2Button)) + String(digitalRead(floor3Button)));
  
}
