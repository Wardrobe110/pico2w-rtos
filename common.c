#include "common.h"

uint32_t global_int32;
EventGroupHandle_t xBtnEventGroup;

float dancefloorBrighness;
struct rgbData dancefloorArray[16];

char uartBuffer[2][32];
uint32_t uartIndex = 0;

struct timespec globalTimer;

int str_to_dec(char *str){
    int value = 0, i = 0;
    do{
        value *= 10;
        value += str[i] - '0';
        i++;
    }while(str[i] != '\0');
    return value;
}

void buttonSetup(){
    gpio_init(LED_0);
    gpio_set_dir(LED_0, true);
    gpio_init(LED_1);
    gpio_set_dir(LED_1, true);
    gpio_init(LED_2);
    gpio_set_dir(LED_2, true);

    //Could be done with for to reduce line number
    gpio_init(BTN_0);
    gpio_set_dir(BTN_0, false);
    gpio_pull_up(BTN_0);
    //Why both rise and fall?
    gpio_set_irq_enabled(BTN_0, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    gpio_init(BTN_1);
    gpio_set_dir(BTN_1, false);
    gpio_pull_up(BTN_1);
    gpio_set_irq_enabled(BTN_1, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    gpio_init(BTN_2);
    gpio_set_dir(BTN_2, false);
    gpio_pull_up(BTN_2);
    gpio_set_irq_enabled(BTN_2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
};



