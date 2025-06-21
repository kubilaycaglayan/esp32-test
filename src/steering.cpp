#include <ESP32Servo.h>

extern Servo steeringServo;

#define SERVO_PIN 14

void setupServo() {
  steeringServo.attach(SERVO_PIN);
  steeringServo.write(90);
  steeringServo.setPeriodHertz(50);
  steeringServo.attach(SERVO_PIN, 200, 3000);
}
