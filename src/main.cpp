#include <Arduino.h>
#include <Ps3Controller.h>
#include "esp_bt_device.h"

#define LED_PIN 4

void notify()
{
    Serial.print(Ps3.data.sensor.accelerometer.x);
    Serial.print(" ");
    Serial.print(Ps3.data.sensor.accelerometer.y);
    Serial.print(" ");
    Serial.print(Ps3.data.sensor.accelerometer.z);

    /* Uncomment the following if you also want
       to display the gryoscope data: */
    // Serial.print(" ");
    // Serial.print(Ps3.data.sensor.gyroscope.z);


}


void setup()
{
    Serial.begin(115200);
    Ps3.attach(notify);
    Ps3.begin("3C:8A:1F:AF:7F:D2");
}


void loop()
{
  if (Ps3.isConnected()) {
    if (Ps3.data.button.cross) {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }
  }

}
