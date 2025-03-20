import paho.mqtt.client as mqtt
import ssl
from django.core.management.base import BaseCommand

# MQTT Broker settings
MQTT_BROKER = "e4995ca1.ala.eu-central-1.emqxsl.com"
MQTT_PORT = 8883  # Secure MQTT port
MQTT_TOPIC = "status"
MQTT_CLIENT_ID = "django_status_listener"
MQTT_USERNAME = "django"
MQTT_PASSWORD = "django"
MQTT_CA_CERT = "/home/lazy/Desktop/zabravihHT11/website/HackTuesXI/myapp/certs/emqxsl-ca.crt"  # Path to your CA certificate

# MQTT Callbacks
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("✅ Connected to EMQX Broker!")
        client.subscribe(MQTT_TOPIC)
    else:
        print(f"❌ Connection failed with code {rc}")

def on_message(client, userdata, msg):
    try:
        status = msg.payload.decode()
        print(f"📥 Received status: {status}")
        # Process the status message as needed
    except Exception as e:
        print(f"⚠️ Error processing message: {e}")

# Django Command to Start MQTT Listener
class Command(BaseCommand):
    help = "Start MQTT client to listen for ESP32 status"

    def handle(self, *args, **kwargs):
        client = mqtt.Client(client_id=MQTT_CLIENT_ID)
        client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
        client.tls_set(ca_certs=MQTT_CA_CERT, cert_reqs=ssl.CERT_REQUIRED)
        client.on_connect = on_connect
        client.on_message = on_message

        try:
            client.connect(MQTT_BROKER, MQTT_PORT, 60)
            client.loop_forever()
        except Exception as e:
            print(f"⚠️ Connection error: {e}")
