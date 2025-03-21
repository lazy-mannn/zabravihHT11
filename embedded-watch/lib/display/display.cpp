#include <display.h>

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];


/*use Arduinos millis() as tick source*/
 static uint32_t my_tick(void){
        return millis();
}

void display_setup(){
        lv_init();

        lv_tick_set_cb(my_tick);

        lv_display_t * disp;
        disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
        lv_display_set_rotation(disp, TFT_ROTATION);

        ui_init();
}

unsigned long lastTickMillis = 0;

void display_loop(){
                unsigned int tickPeriod = millis() - lastTickMillis;
                lv_tick_inc(tickPeriod);
                lastTickMillis = millis();

                lv_timer_handler();
                // delay(1);
}