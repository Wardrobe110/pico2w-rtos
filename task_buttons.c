#include "task_buttons.h"

TaskHandle_t xButtonTaskHandle;

void task_buttons(__unused void *pvParameter){
    BaseType_t notifyValue;
    while(1){
        //Dont clear on entry, wipe on exit, store value here, wait for indefinite time
        xTaskNotifyWait(0x00, 0xFF, &notifyValue, portMAX_DELAY);
        //printf("Button port: %d\n", notifyValue);
        
        
        if(notifyValue & BTN_BIT_0) xEventGroupSetBits(xBtnEventGroup, BTN_BIT_0);
        if(notifyValue & BTN_BIT_1) xEventGroupSetBits(xBtnEventGroup, BTN_BIT_1);
        if(notifyValue & BTN_BIT_2) xEventGroupSetBits(xBtnEventGroup, BTN_BIT_2);
    }
};