#include "task_leds.h"

void task_leds(__unused void *pvParameter){
    while(1){
        //Even group identifier, wait for, dont clear on entry, wipe on exit, dont wait for all, wait for indefinite time
        EventBits_t bits = xEventGroupWaitBits(
            xBtnEventGroup, BTN_BIT_2 | BTN_BIT_1 | BTN_BIT_0, pdTRUE, pdFALSE, portMAX_DELAY
        );
        
        //printf("Led bits: %d\n", bits);
        gpio_put_masked(1UL << LED_2 | 1UL << LED_1 | 1UL << LED_0, bits << LED_0);
    }   
}