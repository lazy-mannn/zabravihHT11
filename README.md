# DoroWatch

A project using Django, EMQX MQTT, and ESP32 to build a Pomodoro-inspired attention management tool.

## About The Project

This project includes:
- A Django backend connected to EMQX MQTT broker
- One ESP32 in a phone case with an accelerometer
- One ESP32 on a watch with a vibration motor
- A web UI to start focus sessions and monitor status

### How It Works
- The user presses "Start Work" on the web interface.
- A 25-minute timer begins.
- If the phone moves during the session, a warning is sent to the watch.
- The watch vibrates.
- The user can press a button on the watch to snooze (trigger a break).
- The server pauses the work timer and starts a 5-minute break.
- If the phone is placed back down (no movement), the watch stops vibrating.

## Built With
<p align="left">
  <a href="https://www.djangoproject.com/" target="_blank"><img src="https://img.shields.io/badge/Django-092E20?style=for-the-badge&logo=django&logoColor=white" /></a>
  <a href="https://www.emqx.com/en/cloud" target="_blank"><img src="https://img.shields.io/badge/EMQX%20Cloud-009688?style=for-the-badge&logo=cloudflare&logoColor=white" /></a>
  <a href="https://www.arduino.cc/" target="_blank"><img src="https://img.shields.io/badge/ESP32-Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white" /></a>
  <a href="https://platformio.org/" target="_blank"><img src="https://img.shields.io/badge/PlatformIO-FF6600?style=for-the-badge&logo=platformio&logoColor=white" /></a>
  <a href="https://developer.mozilla.org/en-US/docs/Web/JavaScript" target="_blank"><img src="https://img.shields.io/badge/JavaScript-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black" /></a>
</p>

## Frontend Use
- Visit https://zabravih.org
- Click "Start Work" to begin a 25-minute session.
- Watch and phone will sync via MQTT.

## Team
- [Ilia Iliev](https://github.com/lazy-mannn)
- [Alexander Grigorov](https://github.com/Mr-TopG)
- [Alexander Beshev](https://github.com/MrBeshev)
- [Alexander Serafimov](https://github.com/sashungera)
- [Antoan Tsonkov](https://github.com/smookie77)
