#include <IRremote.h>

const int enA = 10;
const int motor1pin1 = A0;
const int motor1pin2 = A1;
const int enB = 9;
const int motor2pin1 = A2;
const int motor2pin2 = A3;

const int IR_recv = 6;
#define IR_BUTTON_UP 24
#define IR_BUTTON_LEFT 8
#define IR_BUTTON_RIGHT 90
#define IR_BUTTON_DOWN 82
#define IR_BUTTON_OK 28

#define IR_BUTTON_1 69
const int IR_line_left = 8;
const int IR_line_right = 7;
bool line_start = false;

#define IR_BUTTON_2 70
const int trigpin = 3;
const int echopin = 4;
int distance = 0;
bool sensor_start = false;


void setup() {
  Serial.begin(9600);
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  IrReceiver.begin(IR_recv);

  pinMode(IR_line_left, INPUT);
  pinMode(IR_line_right, INPUT);

  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);

}

void loop() {

  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(enA, 100);  //ENA pin
  analogWrite(enB, 100);  //ENB pin

  if (IrReceiver.decode()) {
    IrReceiver.resume();
    int command = IrReceiver.decodedIRData.command;
    if (command != 0) {
      switch (command) {
        case IR_BUTTON_UP:
          {
            forward();
            break;
          }
        case IR_BUTTON_DOWN:
          {
            backward();
            break;
          }
        case IR_BUTTON_LEFT:
          {
            turnLeft();
            break;
          }
        case IR_BUTTON_RIGHT:
          {
            turnRight();
            break;
          }
        case IR_BUTTON_1:
          {
            if (line_start == false) {
              line_start = true;
            }

            break;
          }
        case IR_BUTTON_2:
          {
            if (sensor_start == false) {
              sensor_start = true;
            }
            break;
          }
      }
    }
    if (command == IR_BUTTON_OK) {
      line_start = false;
      sensor_start = false;
      stop();
    }
  }
  if (line_start == true) {

    if ((digitalRead(IR_line_right) == 0) && (digitalRead(IR_line_left) == 0)) {
      forward();
    }  //if Right Sensor and Left Sensor are at White color then it will call forword function

    if ((digitalRead(IR_line_right) == 1) && (digitalRead(IR_line_left) == 0)) {
      turnRight();
    }
    if ((digitalRead(IR_line_right) == 0) && (digitalRead(IR_line_left) == 1)) {
      turnLeft();
    }

    if ((digitalRead(IR_line_right) == 1) && (digitalRead(IR_line_left) == 1)) {
      stop();
    }  //if Right Sensor and Left Sensor are at Black color then it will call Stop function
  }

  if (sensor_start == true) {
    findDistance();
    if (distance > 35) {
      forward();
    }
    if (distance < 35 && distance > 0) {
      stop();
      backward();
      delay(500);
      stop();
      turnRight();
      stop();
    }
  }
}


void forward() {
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);

  if (line_start == false && sensor_start == false) {
    delay(100);
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
  }
}

void backward() {

  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  if (line_start == false && sensor_start == false) {
    delay(100);
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
  }
}

void turnRight() {
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);

  if (line_start == false && sensor_start == false) {
    delay(100);
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
  }
  if (sensor_start == true) {
    delay(500);
  }
}

void turnLeft() {

  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  if (line_start == false && sensor_start == false) {
    delay(100);
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
  }
  if (sensor_start == true) {
    delay(500);
  }
}

void stop() {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);

  delay(250);
  line_start = false;
}

void findDistance() {
  int duration;
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);

  duration = pulseIn(echopin, HIGH);
  distance = (duration / 2) / 29.1;
  delay(60);
}