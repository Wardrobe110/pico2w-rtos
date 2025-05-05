#include "common.h"
#include "ws2812.pio.h"

#define LEDS_NUMBER 16


extern TaskHandle_t xDancefloorTaskHandle;

void ws2812b_setup();

static inline void put_pixel();
static inline uint32_t urgb_u24(uint8_t r, uint8_t g, uint8_t b);

static void dancefloor_set_example();

void dancefloor_redraw();

void task_dancefloor(__unused void *pvParameter);