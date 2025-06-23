#include <ESP32Servo.h>

extern Servo steeringServo;

#define SERVO_PIN 14

void setupServo() {
  int steeringServoChannel = steeringServo.attach(SERVO_PIN);
  Serial.print("ℹ️ Steering servo attached to channel: ");
  Serial.println(steeringServoChannel);

  steeringServo.setPeriodHertz(50);
  int steeringServoChannel_2 = steeringServo.attach(SERVO_PIN, 200, 3000);
  Serial.print("ℹ️ Steering servo attached to channel (with range): ");
  Serial.println(steeringServoChannel_2);

  steeringServo.write(90);
}
