#include "common.h"

extern TaskHandle_t xUartTaskHandle;
extern int bufferToggle;

static void toggleBuffer();

void uart_interrupt();

void uart_setup();

void task_uart(__unused void *pvParameter);