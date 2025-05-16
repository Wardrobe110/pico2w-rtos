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
#include "hardware/watchdog.h"
#include "hardware/pwm.h"
#include <string.h>
#include "pico/aon_timer.h"



//Structs
struct rgbData{
    uint8_t r;
    uint8_t g;
    uint8_t b;   
};

//Globals
extern uint32_t global_int32;
extern EventGroupHandle_t xBtnEventGroup;
extern float dancefloorBrighness;
extern struct rgbData dancefloorArray[16];
    //Uart globals
#define UART_BUF_LEN 32
extern char uartBuffer[2][32];
extern uint32_t uartIndex;
    //Timer
extern struct timespec globalTimer;

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

#define WS2812B_PIN 15

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 4
#define UART_RX_PIN 5

#define OLED_SDA 12
#define OLED_SCL 13

//Button bits
#define BTN_BIT_0 (1 << 0UL)
#define BTN_BIT_1 (1 << 1UL)
#define BTN_BIT_2 (1 << 2UL)

//Task priorites
#define MAIN_TASK_PRIORITY ( tskIDLE_PRIORITY + 5UL )
#define BUTTON_PRIORITY ( tskIDLE_PRIORITY + 4UL )
#define BLINKER_PRIORITY ( tskIDLE_PRIORITY + 0UL )
#define LED_PRIORITY ( tskIDLE_PRIORITY + 1UL )
#define DANCEFLOOR_PRIORITY ( tskIDLE_PRIORITY + 2UL )
#define UART_PRIORITY ( tskIDLE_PRIORITY + 4UL )

//Task stacks
#define MAIN_STACK_SIZE 5096
#define BUTTON_STACK_SIZE 512
#define BLINKER_STACK_SIZE 512
#define LED_STACK_SIZE 512
#define DANCEFLOOR_STACK_SIZE 1024
#define UART_STACK_SIZE 1024

int str_to_dec(char *str);

void buttonSetup();