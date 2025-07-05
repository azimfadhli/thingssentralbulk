#include <thingssentralbulk.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* serverURL = "http://myIot.com.e";

// Create multiple bulk senders
ThingsSentralBulk sensorA(serverURL, "LivingRoom");
ThingsSentralBulk sensorB(serverURL, "Kitchen");

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  // Configure NTP once (GMT+8)
  ThingsSentralBulk::configureNTP(8);
}

void loop() {
  static unsigned long lastSample = 0;

  // Sample every second
  if (millis() - lastSample >= 1000) {
    sensorA.addData(36 /* readSensorA() */);
    sensorB.addData(25 /* readSensorB() */);
    lastSample = millis();
  }

  // Send bulk data every minute
  static unsigned long lastSend = 0;
  if (millis() - lastSend >= 60000) {
    if (sensorA.send()) Serial.println("Sensor A sent");
    if (sensorB.send()) Serial.println("Sensor B sent");
    lastSend = millis();
  }
}