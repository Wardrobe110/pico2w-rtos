#include "common.h"
#include "task_buttons.h"
#include "task_blinker.h"
#include "task_leds.h"
#include "task_dancefloor.h"
#include "task_uart.h"

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
//============================
//I need to use dma here
//Clock div works at least for now
//Could also be timer interrupt

//There is no need for all that
//just use read in the main loop
void adc_interrupt(){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
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
    xTaskNotifyFromISR(xDancefloorTaskHandle, 0, eNoAction, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    
}

//Will be moved to common
void general_setup(){
    buttonSetup();
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

    uart_setup();
    memset(&globalTimer, 0, sizeof(globalTimer));
    aon_timer_start(&globalTimer);
}

void main_task(void *ptr){

    xBtnEventGroup = xEventGroupCreate();

    xTaskCreate(task_blinker, "blinker", BLINKER_STACK_SIZE, NULL, BLINKER_PRIORITY, _NULL);
    xTaskCreate(task_buttons, "buttons", BUTTON_STACK_SIZE, NULL, BUTTON_PRIORITY, &xButtonTaskHandle);
    xTaskCreate(task_leds, "leds", LED_STACK_SIZE, NULL, LED_PRIORITY, _NULL);
    xTaskCreate(task_dancefloor, "dancefloor", DANCEFLOOR_STACK_SIZE, NULL, DANCEFLOOR_PRIORITY, &xDancefloorTaskHandle);
    xTaskCreate(task_uart, "uart", UART_STACK_SIZE, NULL, UART_PRIORITY, &xUartTaskHandle);
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
    ws2812b_setup();
    vLaunch();

    while(1){
        sleep_ms(1000);
    }
    return 0;
}