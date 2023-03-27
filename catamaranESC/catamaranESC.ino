// Катамаран ESC + HC-06
#include <Servo.h>
Servo motorLeft;
Servo motorRight;

int maxSpeedLeft[2] = { 1000, 2000 };
int maxSpeedRight[2] = { 2000, 1000 };
const int neutralPosition = 1500;

int directionFlag = 0;
int _speedLeft = maxSpeedLeft[directionFlag];
int _speedRight = maxSpeedRight[directionFlag];

void setup() {
  Serial.begin(9600);
  motorLeft.attach(9);
  motorLeft.write(neutralPosition);

  motorRight.attach(3);
  motorRight.write(neutralPosition);
  delay(1000);
}

void loop() {
  if (Serial.available() != 0) {
    char valueRead = Serial.read();
    if (valueRead >= '0' && valueRead <= '9') {
      _speedLeft = map((valueRead - '0'), 0, 9, neutralPosition, maxSpeedLeft[directionFlag]);
      _speedRight = map((valueRead - '0'), 0, 9, neutralPosition, maxSpeedRight[directionFlag]);
    } else if (valueRead == 'F') forward(_speedLeft, _speedRight);
    else if (valueRead == 'B') backward();
    else if (valueRead == 'L') left();
    else if (valueRead == 'R') right();
    else if (valueRead == 'G') forwardLeft(_speedLeft, _speedRight);
    else if (valueRead == 'I') forwardRight(_speedLeft, _speedRight);
    else if (valueRead == 'H') backLeft(_speedLeft, _speedRight);
    else if (valueRead == 'J') backRight(_speedLeft, _speedRight);

    else if (valueRead == 'W') directionFlag = 1;
    else if (valueRead == 'w') directionFlag = 0;
    else stop();
  }
}



void forward(int _sLeft, int _sRight) {
  motorLeft.writeMicroseconds(_sLeft);
  motorRight.writeMicroseconds(_sRight);
}
void backward() {
  if (directionFlag == 1) {
    motorLeft.writeMicroseconds(neutralPosition - 200);
    motorRight.writeMicroseconds(neutralPosition + 200);
  } else {
    motorLeft.writeMicroseconds(neutralPosition + 200);
    motorRight.writeMicroseconds(neutralPosition - 200);
  }
}
void left() {
  if (directionFlag == 1) {
    motorLeft.writeMicroseconds(maxSpeedRight[directionFlag]);
    motorRight.writeMicroseconds(maxSpeedRight[directionFlag]);
  } else {
    motorRight.writeMicroseconds(maxSpeedLeft[directionFlag]);
    motorLeft.writeMicroseconds(maxSpeedLeft[directionFlag]);
  }
}
void right() {
  if (directionFlag == 0) {
    motorLeft.writeMicroseconds(maxSpeedRight[directionFlag]);
    motorRight.writeMicroseconds(maxSpeedRight[directionFlag]);
  } else {
    motorRight.writeMicroseconds(maxSpeedLeft[directionFlag]);
    motorLeft.writeMicroseconds(maxSpeedLeft[directionFlag]);
  }
}
void forwardLeft(int _sLeft, int _sRight) {
  motorLeft.writeMicroseconds(_sLeft * 0.5);
  motorRight.writeMicroseconds(_sRight);
}
void forwardRight(int _sLeft, int _sRight) {
  motorLeft.writeMicroseconds(_sLeft);
  motorRight.writeMicroseconds(_sRight * 0.5);
}
void backLeft(int _sLeft, int _sRight) {
  motorLeft.writeMicroseconds(_sRight * 0.5);
  motorRight.writeMicroseconds(_sLeft);
}
void backRight(int _sLeft, int _sRight) {
  motorLeft.writeMicroseconds(_sRight);
  motorRight.writeMicroseconds(_sLeft * 0.5);
}
void stop() {
  motorLeft.write(neutralPosition);
  motorRight.write(neutralPosition);
}