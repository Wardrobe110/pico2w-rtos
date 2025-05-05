#include "task_dancefloor.h"

PIO pio;
uint sm;
uint offset;

TaskHandle_t xDancefloorTaskHandle;

void ws2812b_setup(){
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, WS2812B_PIN, 1, true);
    hard_assert(success);
    gpio_set_function(WS2812B_PIN, GPIO_FUNC_PIO0);
    
    ws2812_program_init(pio, sm, offset, WS2812B_PIN, 800000, false);
    dancefloor_set_example();
};

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

static inline uint32_t urgb_u24(uint8_t r, uint8_t g, uint8_t b) {
    return (((uint32_t)g << 16u) | ((uint32_t)r << 8u) | b);
}

static void dancefloor_set_example(){
    for(int i = 0; i < LEDS_NUMBER; i++){
        dancefloorArray[i].r = 255 - i * 16;
        dancefloorArray[i].g = 0;
        dancefloorArray[i].b = i * 16;
    }
}

void dancefloor_redraw(){
    for(int i = 0; i < LEDS_NUMBER; i++){
        put_pixel(urgb_u24(dancefloorArray[i].r * dancefloorBrighness / 100,
             dancefloorArray[i].g * dancefloorBrighness / 100,
              dancefloorArray[i].b * dancefloorBrighness / 100));
    }
    sleep_us(80);
}

void task_dancefloor(__unused void *pvParameter){
    while(1){
        //Dont clear on entry, wipe on exit, store value here, wait for indefinite time
        xTaskNotifyWait(0x00, 0xFF, NULL, portMAX_DELAY);
        //printf("Yello!\n");
        dancefloor_redraw();
    }
};