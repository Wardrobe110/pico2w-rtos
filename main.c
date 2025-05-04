#include "common.h"
#include "task_buttons.h"
#include "task_blinker.h"


void btn_interrupt(uint gpio, uint32_t event_mask){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int btn = 0;
    if(gpio == BTN_0){
        btn = BTN_0;
    }else if(gpio == BTN_1){
        btn = BTN_0;
    }else if(gpio == BTN_2){
        btn = BTN_0;
    };
    xTaskNotifyFromISR(xButtonTaskHandle, btn, eSetBits, &xHigherPriorityTaskWoken);
};

void general_setup(){
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
    gpio_set_irq_enabled(BTN_0, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    gpio_init(BTN_1);
    gpio_set_dir(BTN_1, false);
    gpio_pull_up(BTN_1);
    gpio_set_irq_enabled(BTN_1, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    gpio_init(BTN_2);
    gpio_set_dir(BTN_2, false);
    gpio_pull_up(BTN_2);
    gpio_set_irq_enabled(BTN_2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    gpio_set_irq_enabled_with_callback(BTN_0, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_interrupt);

    //Method for insane ppl
    /*
    gpio_set_irq_callback(&btn_interrupt);
    irq_set_enabled(IO_IRQ_BANK0, true);
    */

    cyw43_arch_init();
    cyw43_arch_gpio_put(LED_P, 0);
    
}

void main_task(void *ptr){

    xISREventGroup = xEventGroupCreate();

    xTaskCreate(task_buttons, "task_btn", BUTTON_STACK_SIZE, NULL, BUTTON_PRIORITY, xButtonTaskHandle);
    xTaskCreate(task_blinker, "blinker", BLINKER_STACK_SIZE, NULL, BLINKER_PRIORITY, _NULL);

    /*
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
    */
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