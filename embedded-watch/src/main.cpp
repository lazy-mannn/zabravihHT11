#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <secrets.h>
#include <display.h>


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

    Serial.print("Message: ");
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char) payload[i];  // Convert payload to a string
    }
    Serial.println(message);
    Serial.println("-----------------------");

    // Example: Perform actions based on received messages
    if (message == "1") {
        Serial.println("1");
    } else if (message == "0") {
        Serial.println("0");
    }
}

  


void setup() {
    Serial.begin(115200);
    setup_wifi();
    timeClient.begin();

    // Set SSL/TLS Certificate
    espClient.setCACert(ca_cert);

    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(mqttCallback);

    // Synchronize time with NTP
    timeClient.update();
    display_setup();
}



unsigned long lastMQTTCheck = 0;
const unsigned long mqttInterval = 1000; // 1 second

void loop() {
	unsigned long now = millis();

	if (now - lastMQTTCheck >= mqttInterval) {
		lastMQTTCheck = now;

		// Run every 1 second
		if (!mqtt_client.connected()) {
			connectToMQTT();
		}
		mqtt_client.loop();

	} else {
		// Run in between MQTT checks
		display_loop();
	}
}

