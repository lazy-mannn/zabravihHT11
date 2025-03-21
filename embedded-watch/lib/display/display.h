#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <ui.h>

 /*Set to your screen resolution and rotation*/
 #define TFT_HOR_RES   240
 #define TFT_VER_RES   240
 #define TFT_ROTATION  LV_DISPLAY_ROTATION_90



void display_setup();
void display_loop();


#endif //DISPLAY_H_