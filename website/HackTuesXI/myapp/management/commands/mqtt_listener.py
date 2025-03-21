import paho.mqtt.client as mqtt
import ssl
import threading
import time
from django.core.management.base import BaseCommand
from myapp.management.managers.timer_status_manager import Status
from myapp.management.managers.timer import TimerManager
from myapp.management.managers.status_db_manager import StatusDBManager

# MQTT Broker settings
MQTT_BROKER = "e4995ca1.ala.eu-central-1.emqxsl.com"
MQTT_PORT = 8883
MQTT_TOPIC = "status"
MQTT_CLIENT_ID = "django_status_listener"
MQTT_USERNAME = "django"
MQTT_PASSWORD = "django"
MQTT_CA_CERT = "myapp/certs/emqxsl-ca.crt"

status_manager = StatusDBManager()
timer_manager = TimerManager(status_manager)

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to EMQX Broker!")
        client.subscribe(MQTT_TOPIC)
    else:
        print(f"Connection failed with code {rc}")

def on_message(client, userdata, msg):
    try:
        status = msg.payload.decode().strip()
        print(f"Received status: {status}")

        if status == "1":
            if status_manager.get_status() == Status.WORK_IN_PROGRESS.name:
                print("Work warning: User moved phone during work session!")
                client.publish(MQTT_TOPIC, "ww")

        elif status == "0":
            print("User stopped moving. Stopping vibration.")
            client.publish(MQTT_TOPIC, "stop")

        elif status == "SNZ":
            print("Snooze activated. Pausing work timer.")
            status_manager.set_status(Status.PAUSED.name)
            client.publish(MQTT_TOPIC, "ww")

    except Exception as e:
        print(f"Error processing message: {e}")

def timer_loop():
    while True:
        timer_manager.tick()
        time.sleep(1)

class Command(BaseCommand):
    help = "Start MQTT client to listen for ESP32 status"

    def handle(self, *args, **kwargs):
        client = mqtt.Client(client_id=MQTT_CLIENT_ID)
        client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
        client.tls_set(ca_certs=MQTT_CA_CERT, cert_reqs=ssl.CERT_REQUIRED)
        client.on_connect = on_connect
        client.on_message = on_message

        # Start the timer loop in a separate thread
        threading.Thread(target=timer_loop, daemon=True).start()

        try:
            client.connect(MQTT_BROKER, MQTT_PORT, 60)
            client.loop_forever()
        except Exception as e:
            print(f"Connection error: {e}")
