#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

#include <WiFi.h>

const char* ssid = "Zabravih";         // Replace with your WiFi SSID
const char* password = "Alex2009"; // Replace with your WiFi Password

// Assign a unique ID
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

void displaySensorDetails() {
    sensor_t sensor;
    accel.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print("Sensor:       "); Serial.println(sensor.name);
    Serial.print("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
    Serial.print("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
    Serial.print("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
    Serial.println("------------------------------------\n");
    delay(500);
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Wire.begin(27, 26);  // SDA = GPIO 21, SCL = GPIO 22
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("\nConnected to Wi-Fi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Accelerometer Test\n");

    // Initialize the sensor
    if (!accel.begin()) {
        Serial.println("Ooops, no LSM303 detected... Check your wiring!");
        while (1);
    }

    displaySensorDetails();
}

void loop() {
    sensors_event_t event;
    accel.getEvent(&event);

    // Print acceleration data
    Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.println(" m/s^2");

    delay(500);
}
