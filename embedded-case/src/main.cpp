#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <secrets.h>


// NTP Server
const long utcOffsetInSeconds = 3600;  // Set your UTC offset (e.g., 3600 for UTC+1)
const char* ntpServer = "pool.ntp.org";  // NTP server (use the default pool or specify your own)
WiFiUDP udp;
NTPClient timeClient(udp, ntpServer, utcOffsetInSeconds);

// Wi-Fi client for MQTT connection
WiFiClientSecure espClient;
PubSubClient mqtt_client(espClient);

// Wi-Fi connection setup
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

  // Check DNS resolution for NTP server
  Serial.print("Resolving DNS for NTP server: ");
  IPAddress ntpIP;
  if (WiFi.hostByName(ntpServer, ntpIP)) {
    Serial.print("Resolved IP: ");
    Serial.println(ntpIP);
  } else {
    Serial.println("DNS resolution failed for NTP server");
  }

  // Check DNS resolution for MQTT broker
  Serial.print("Resolving DNS for MQTT broker: ");
  IPAddress mqttBrokerIP;
  if (WiFi.hostByName(mqtt_broker, mqttBrokerIP)) {
    Serial.print("Resolved IP: ");
    Serial.println(mqttBrokerIP);
  } else {
    Serial.println("DNS resolution failed for MQTT broker");
  }
}

// MQTT connection setup with SSL/TLS
void connectToMQTT() {
  String client_id = "esp32-client-" + String(WiFi.macAddress());
  Serial.printf("Connecting to MQTT Broker as %s...\n", client_id.c_str());

  if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker!");
      mqtt_client.subscribe(mqtt_topic);
      mqtt_client.publish(mqtt_topic, "Connected");
  } else {
      Serial.print("Failed to connect, MQTT error code: ");
      Serial.println(mqtt_client.state());  // Shows the connection failure reason
      delay(5000);  // Retry after 5 seconds
  }
}

// MQTT message callback
void mqttCallback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (unsigned int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

void displaySensorDetails() {
    sensor_t sensor;
    accel.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print("Sensor:       "); Serial.println(sensor.name);
    Serial.print("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print("Max Value:    "); Serial.println(sensor.max_value);
    Serial.print("Min Value:    "); Serial.println(sensor.min_value);
    Serial.print("Resolution:   "); Serial.println(sensor.resolution);
    Serial.println("------------------------------------");
}

int status(int check) {
  sensors_event_t event;
  accel.getEvent(&event);

  int x = event.acceleration.x;
  int y = event.acceleration.y;

  // Serial.print("\nX: "); Serial.print(x);
  // Serial.print(" Y: "); Serial.print(y);

  if ((x>-3 && x<3 && y>-3 && y<3) || (x>-3 && x<3 && (y>8 || y<-8)) || ((x>8 || x<-8) && y>-3 && y<3))
    check = 0;      //no movement
  else
    check = 1;      //movement
  
  return check;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(27, 26);  // SDA = GPIO 27, SCL = GPIO 26
  setup_wifi();
  timeClient.begin();

  // Initialize Accelerometer
  if (!accel.begin()) {
      Serial.println("Could not find a valid LSM303 sensor, check wiring!");
      while (1);
  }
  displaySensorDetails();
  
  // Set SSL/TLS Certificate
  espClient.setCACert(ca_cert);
  
  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setCallback(mqttCallback);

  // Synchronize time with NTP
  timeClient.update();
}

void loop() {
  // Ensure Wi-Fi is connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi lost, reconnecting...");
    WiFi.disconnect();
    setup_wifi();
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWi-Fi Reconnected!");
  }

  // Ensure MQTT is connected
  if (!mqtt_client.connected()) {
    Serial.println("MQTT lost, reconnecting...");
    connectToMQTT();
  }
  mqtt_client.loop();

  static int send = -1; // Initialize send to an impossible state (not 0 or 1)
  int check = status(0); // Call status() and store the return value

  if (send != check) { // Only send if the status changes
    String message = String(check);
    mqtt_client.publish(mqtt_topic, message.c_str());
    //if (check == 1)
    //    mqtt_client.publish(mqtt_topic, "ww");
    //else
    //    mqtt_client.publish(mqtt_topic, "stop");
    Serial.println("Status changed");
    send = check;  // Store the new status
  }
  delay(500);  // Delay 1 second before checking again
}
