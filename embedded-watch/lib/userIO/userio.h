#ifndef USERIO_H_
#define USERIO_H_

#include <Arduino.h>

extern int ledGreen_value;
extern int ledRed_value;
extern int notifier_value;
extern int snooze_state;

#define LED_GREEN_PIN 12
#define LED_RED_PIN 14
#define NOTIFIER_PIN 27

#define SNZ_BTN 16

void userio_setup();
void userio_loop();


#endif //USERIO_H_