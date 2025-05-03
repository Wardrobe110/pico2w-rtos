#pragma ONCE
#include "FreeRTOS.h"
#include "event_groups.h"
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "task.h"
#include "pico/async_context_freertos.h"

extern EventGroupHandle_t xISREventGroup;
extern uint32_t global_int32;

//Pin defines
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 4
#define UART_RX_PIN 5

#define LED_0 16
#define LED_1 17
#define LED_2 18
#define LED_P CYW43_WL_GPIO_LED_PIN

#define BTN_0 19
#define BTN_1 20
#define BTN_2 21


//Task priorites
#define MAIN_TASK_PRIORITY 9
#define BUTTON_PRIORITY 4


//Task stacks
#define MAIN_STACK_SIZE 5096
#define BUTTON_STACK_SIZE 512