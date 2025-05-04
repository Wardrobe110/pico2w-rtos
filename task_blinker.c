#include "task_blinker.h"

void task_blinker(__unused void *pvParameters){
    bool isOn = false;
    while(1){
        cyw43_arch_gpio_put(LED_P,isOn);
        isOn = !isOn;
        sleep_ms(500);
    }
};