#include "common.h"
#include "task_buttons.h"



int main(){
    stdio_init_all();
    
    
    return 0;
}

void pins_setup(){
    //GPIO_

}


void main_task(void *ptr){




    xISREventGroup = xEventGroupCreate();

    xTaskCreate(task_buttons, "task_btn", BUTTON_STACK_SIZE ,NULL, BUTTON_PRIORITY, _NULL);

    int count = 0;
    while(true) {
        static int last_core_id = -1;
        if (portGET_CORE_ID() != last_core_id) {
            last_core_id = portGET_CORE_ID();
            printf("main task is on core %d\n", last_core_id);
        }
        printf("Hello from main task count=%u\n", count++);
        vTaskDelay(3000);
    }

}

void vLaunch(void){
    TaskHandle_t task;
    xTaskCreate(main_task, "MainThread", MAIN_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL);
    vTaskCoreAffinitySet(task, 1);
    vTaskStartScheduler();
}