#include "task_uart.h"

TaskHandle_t xUartTaskHandle;
int bufferToggle = 0;
static void toggleBuffer(){
    
    bufferToggle = !bufferToggle;
}

void uart_interrupt(){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    while (uart_is_readable(UART_ID)) {
        //led_toggle(); // Let’s have some fun
        uint8_t ch = uart_getc(UART_ID);
        if(ch == '\r') {
            //process_command();
            printf("%s",uartBuffer[0]);
            xTaskNotifyFromISR(xUartTaskHandle, 0, eNoAction, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            //TODO MANAGE BUFFER CHANGE 
            //uartIndex = 0; // Prepare for another command reception
            //uartBuffer[0][uartIndex] = '\0';  // even empty C string must end with zero (\0)
        } else {
            if(uartIndex<UART_BUF_LEN-1) {
                uartBuffer[0][uartIndex++] = ch;
                uartBuffer[0][uartIndex] = '\0'; // ASCIIZ - C string must end with zero (\0)
            } else {
                ch = '~'; // Notify user: term buffer is full
            }
            /*
            if (term_echo && uart_is_writable(RS485_UART_ID)) {
                uart_write_blocking(RS485_UART_ID, (const uint8_t *) &ch, 1); // Send it back
                uart_tx_wait_blocking(RS485_UART_ID); //wait fifo empty (even we don’t use it)
            }
            */
        }
    }
};

void uart_setup(){
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
        //Handler setup
    irq_set_exclusive_handler(UART0_IRQ, uart_interrupt);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);
};



void task_uart(__unused void *pvParameter){
    char tmp[32];
    char *tmpPtr;
    while (1){
        xTaskNotifyWait(0x00, 0xFF, NULL, portMAX_DELAY);
        int bufferIndex = 0;
        if(strcmp(uartBuffer[bufferIndex], "help") == 0){
            uart_puts(UART_ID, "Availble functions:\n");
            uart_puts(UART_ID, "reboot // reboots system\n");
            uart_puts(UART_ID, "uptime // prints uptime\n");
            uart_puts(UART_ID, "led N state // N = 0, 1, 2 state = 0, 1\n");
            uart_puts(UART_ID, "leds bitmask // bitmask = 0b000 ... 0b111\n");
            uart_puts(UART_ID, "pwn N value // N = 0, 1, 2 value 0 ... 100\n");
            uart_puts(UART_ID, "oled separated|text // prints text on oled\n");
            uart_puts(UART_ID, "ws N RRGGBB // N = 0...15 RR GG BB = 0x00..FF\n");
        }else if(strcmp(uartBuffer[bufferIndex], "reboot") == 0){
            uart_puts(UART_ID, "Rebooting...\n");
            watchdog_reboot(0, 0, 0);
            watchdog_enable(100, 0);
            while(1);  
        }else if(strcmp(uartBuffer[bufferIndex], "uptime") == 0){
            /*
            aon_timer_get_time(&globalTimer);
            sprintf(tmp, "Uptime: %d seconds\n", globalTimer.tv_sec);
            uart_puts(UART_ID, tmp);
            */
        }else{
            tmpPtr = strtok(uartBuffer[bufferIndex], " ");
            if(strcmp(tmpPtr, "led") == 0){
                uart_puts(UART_ID, "led\n");
                tmpPtr = strtok(NULL, " ");
                switch (str_to_dec(tmpPtr)){
                case 0:
                    tmpPtr = strtok(NULL, " ");
                    gpio_set_function(LED_0, GPIO_FUNC_SIO);
                    gpio_put(LED_0, str_to_dec(tmpPtr));
                    break;
                case 1:
                    tmpPtr = strtok(NULL, " ");
                    gpio_set_function(LED_1, GPIO_FUNC_SIO);
                    gpio_put(LED_1, str_to_dec(tmpPtr));
                    break;
                case 2:
                    tmpPtr = strtok(NULL, " ");
                    gpio_set_function(LED_2, GPIO_FUNC_SIO);
                    gpio_put(LED_2, str_to_dec(tmpPtr));
                    break;
                default:
                    uart_puts(UART_ID, "Wrong led ID\n");
                    break;
                }
            }else if(strcmp(tmpPtr, "leds") == 0){
                tmpPtr = strtok(NULL, " ");
                //check for binary value maybe?
                gpio_set_function(LED_0, GPIO_FUNC_SIO);
                gpio_set_function(LED_1, GPIO_FUNC_SIO);
                gpio_set_function(LED_2, GPIO_FUNC_SIO);
                gpio_put(LED_0, tmpPtr[2] - '0');
                gpio_put(LED_1, tmpPtr[3] - '0');
                gpio_put(LED_2, tmpPtr[4] - '0');
            }else if(strcmp(tmpPtr, "pwm") == 0){
                tmpPtr = strtok(NULL, " ");
                switch (str_to_dec(tmpPtr)){
                    case 0:
                        tmpPtr = strtok(NULL, " ");
                        gpio_set_function(LED_0, GPIO_FUNC_PWM);
                        pwm_set_gpio_level(LED_0, str_to_dec(tmpPtr));
                        break;
                    case 1:
                        tmpPtr = strtok(NULL, " ");
                        gpio_set_function(LED_1, GPIO_FUNC_PWM);
                        pwm_set_gpio_level(LED_1, str_to_dec(tmpPtr));
                        break;
                    case 2:
                        tmpPtr = strtok(NULL, " ");
                        gpio_set_function(LED_2, GPIO_FUNC_PWM);
                        pwm_set_gpio_level(LED_2, str_to_dec(tmpPtr));
                        break;
                    default:
                        uart_puts(UART_ID, "Wrong led ID\n");
                        break;
                    }
            }else if(strcmp(tmpPtr, "oled") == 0){
                //Clearing screen
                /*
                memset(buf, 0, SSD1306_BUF_LEN);
                render(buf, &frame_area);
                tmpPtr = strtok(NULL, "|");
                uint8_t y = 0;
                while(tmpPtr != NULL){
                    WriteString(buf, 0, y, tmpPtr);             
                    tmpPtr = strtok(NULL, "|");
                    y += 8;
                }
                render(buf, &frame_area);
                */
            }
            else if(strcmp(tmpPtr, "ws") == 0){
                /*
                //Clearing screen
                uart_puts(UART_ID, "Dance bby");
                put_pixel(pio, sm, urgb_u32(0,0,200));
                */
            }else{
                uart_puts(UART_ID, "Unknown command\n");
            }
        }
    }
};