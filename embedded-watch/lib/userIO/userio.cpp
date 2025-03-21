#include <userio.h>

int ledGreen_value = 0;
int ledRed_value   = 0;
int notifier_value = 0;
int snooze_state = 0;

void userio_setup(){
        pinMode(LED_GREEN_PIN, OUTPUT);
        pinMode(LED_RED_PIN, OUTPUT);
        pinMode(NOTIFIER_PIN, OUTPUT);
        pinMode(SNZ_BTN, INPUT_PULLUP);
}

void userio_loop(){
        digitalWrite(LED_GREEN_PIN, ledGreen_value);
        digitalWrite(LED_RED_PIN, ledRed_value);
        digitalWrite(NOTIFIER_PIN, notifier_value);
        snooze_state = (digitalRead(SNZ_BTN) == LOW) ? 1 : 0;
}