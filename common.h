#pragma once
#include "FreeRTOS.h"
#include "event_groups.h"
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "task.h"
#include "pico/async_context_freertos.h"
#include "pico/cyw43_arch.h"
#include "timers.h"
#include "hardware/adc.h"

//Globals
extern uint32_t global_int32;
extern EventGroupHandle_t xBtnEventGroup;

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

#define KNOB_PIN 26
#define SENSITIVITY 5

//Button bits
#define BTN_BIT_0 (1 << 0UL)
#define BTN_BIT_1 (1 << 1UL)
#define BTN_BIT_2 (1 << 2UL)

//Task priorites
#define MAIN_TASK_PRIORITY ( tskIDLE_PRIORITY + 5UL )
#define BUTTON_PRIORITY ( tskIDLE_PRIORITY + 4UL )
#define BLINKER_PRIORITY ( tskIDLE_PRIORITY + 0UL )
#define LED_PRIORITY ( tskIDLE_PRIORITY + 1UL )

//Task stacks
#define MAIN_STACK_SIZE 5096
#define BUTTON_STACK_SIZE 512
#define BLINKER_STACK_SIZE 512
#define LED_STACK_SIZE 512