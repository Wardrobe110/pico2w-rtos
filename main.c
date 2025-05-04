#include "common.h"
#include "task_buttons.h"
#include "task_blinker.h"
#include "task_leds.h"


void btn_interrupt(uint gpio, uint32_t event_mask){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int btn = 0;
    if(gpio == BTN_0){
        btn = BTN_BIT_0;
    }else if(gpio == BTN_1){
        btn = BTN_BIT_1;
    }else if(gpio == BTN_2){
        btn = BTN_BIT_2;
    };
    xTaskNotifyFromISR(xButtonTaskHandle, btn, eSetBits, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
};

//I need to use dma here
//Clock div works at least for now
void adc_interrupt(){
    static volatile uint16_t prevRead = 0;
    uint16_t result = adc_fifo_get();

    //This might just be insanely stupid idea?
    //I highly doubt this saves cycles
    if(abs((int)prevRead - (int)result) < SENSITIVITY) return;
    prevRead = result;

    //printf("Adc input: %d", result);
    
    //There has to be a way to cleanly map this to intigers from 0 to 100
    //sth like clamp down(result/2.53)?
    dancefloorBrighness = result / 2.55;

    //printf("Calc val: %f\n", dancefloorBrighness);
}


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

    gpio_set_irq_enabled_with_callback(BTN_0, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_interrupt);

    //Method for insane ppl
    /*
    gpio_set_irq_callback(&btn_interrupt);
    irq_set_enabled(IO_IRQ_BANK0, true);
    */

    cyw43_arch_init();
    cyw43_arch_gpio_put(LED_P, 0);
    
    adc_init();
    adc_gpio_init(KNOB_PIN);
    gpio_set_function(KNOB_PIN, GPIO_FUNC_NULL);
    gpio_disable_pulls(KNOB_PIN);
    gpio_set_input_enabled(KNOB_PIN, false);
    
    //We byte shift it by degault
    adc_fifo_setup(true, false, 1, false, true);
    adc_irq_set_enabled(true);
    irq_set_exclusive_handler(ADC_IRQ_FIFO, adc_interrupt); // Set ISR
    irq_set_enabled(ADC_IRQ_FIFO, true); 
    //5 times per second assuming 150MHz clock
    adc_set_clkdiv(312500);
    //Might need to be moved in case we use more than 1 adc
    adc_select_input(KNOB_PIN - 26);

    adc_run(true);
}

void main_task(void *ptr){

    xBtnEventGroup = xEventGroupCreate();

    xTaskCreate(task_blinker, "blinker", BLINKER_STACK_SIZE, NULL, BLINKER_PRIORITY, _NULL);
    xTaskCreate(task_buttons, "buttons", BUTTON_STACK_SIZE, NULL, BUTTON_PRIORITY, &xButtonTaskHandle);
    xTaskCreate(task_leds, "leds", LED_STACK_SIZE, NULL, LED_PRIORITY, _NULL);

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