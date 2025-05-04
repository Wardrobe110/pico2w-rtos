#include "common.h"
#include "task_buttons.h"
#include "task_blinker.h"

void general_setup(){
    gpio_init(LED_0);
    gpio_set_dir(LED_0, true);
    gpio_init(LED_1);
    gpio_set_dir(LED_1, true);
    gpio_init(LED_2);
    gpio_set_dir(LED_2, true);

    gpio_init(BTN_0);
    gpio_set_dir(BTN_0, false);
    gpio_init(BTN_1);
    gpio_set_dir(BTN_1, false);
    gpio_init(BTN_2);
    gpio_set_dir(BTN_2, false);

    cyw43_arch_init();
    cyw43_arch_gpio_put(LED_P, 0);
}

void main_task(void *ptr){




    xISREventGroup = xEventGroupCreate();

    //xTaskCreate(task_buttons, "task_btn", BUTTON_STACK_SIZE, NULL, BUTTON_PRIORITY, _NULL);
    xTaskCreate(task_blinker, "blinker", BLINKER_STACK_SIZE, NULL, BLINKER_PRIORITY, _NULL);

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

void vLaunch(){
    TaskHandle_t task;
    xTaskCreate(main_task, "main", MAIN_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL);
    vTaskCoreAffinitySet(task, 1);
    vTaskStartScheduler();
}

int main(){
    stdio_init_all();
    general_setup();
    vLaunch();
    
    return 0;
}