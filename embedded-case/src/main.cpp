#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <sram.h>
#include <SPI.h>


const char* mqtt_cert = \
"-----BEGIN CERTIFICATE----- \n" \
"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \ 
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
"-----END CERTIFICATE-----\n";

// MQTT Broker settings
const char* mqtt_server = "e4995ca1.ala.eu-central-1.emqxsl.com"; 
const int mqtt_port = 8883;
const char* publish_topic = "status";

// WiFi settings
const char* ssid = "Zabravih";  
const char* password = "Alex2009";

// MQTT Username and Password
const char* mqtt_user = "esp32-u";    // Replace with your MQTT username
const char* mqtt_pass = "esp32-pass";    // Replace with your MQTT password



// Wi-Fi client for MQTT connection
WiFiClientSecure espClient;
PubSubClient client(espClient);

// Declare SRAM object (adjust parameters for your SRAM module)
SRAM ram(14);  // Chip select pin = D14
uint16_t addr = 0;
uint8_t value = 0x0A;
uint8_t res = 0;

void setup_wifi() {
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  for (unsigned int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");

      // Attempt to connect with a client ID
      if (client.connect("ESP32Client")) {
          Serial.println("connected");
          client.subscribe("sensor/acceleration");  // Subscribe to a topic (optional)
      } else {
          Serial.print("failed, rc=");
          Serial.print(client.state());
          Serial.println(" retrying in 5 seconds...");
          delay(5000);
      }
  }
}

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
    setup_wifi();
   
    Serial.println("Accelerometer Test\n");

    // Initialize the sensor
    if (!accel.begin()) {
        Serial.println("Ooops, no LSM303 detected... Check your wiring!");
        while (1);
    }

    displaySensorDetails();

    espClient.setCertificate(mqtt_cert);

    // Set the MQTT server and callback
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);


     // Initialize SRAM if needed (example for external SRAM storage)
     ram.begin();
     ram.writeByte(addr, value);  // Write initial value to SRAM

     reconnect();
  }

void loop() {
    sensors_event_t event;
    accel.getEvent(&event);

    if (!client.connected()) {
      reconnect();
    }
    client.loop();

    client.publish(publish_topic, "test");


     // Store sensor data in SRAM
     ram.writeByte(addr++, event.acceleration.x);  // Example write to SRAM (adjust logic)

    // Print acceleration data
    //Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
    //Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
    //Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.println(" m/s^2");

      // Read and print value from SRAM (for example, print last written value)
    res = ram.readByte(addr - 1);  // Read previous value from SRAM
    //Serial.print("Last value from SRAM: ");
    //Serial.println(res, HEX);

    delay(500);
}
