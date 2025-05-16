# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
set(FREERTOS_KERNEL_PATH D:/Pliki/Pico/Rtos/FreeRTOS-Kernel-main)
include(pico_sdk_import.cmake)

project(Rtos C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()
include(FreeRTOS_Kernel_import.cmake)

# Add executable. Default name is the project name, version 0.1

add_executable(Rtos main.c common.c task_blinker.c task_buttons.c task_leds.c task_dancefloor.c ws2812.pio task_uart.c)

pico_set_program_name(Rtos "Rtos")
pico_set_program_version(Rtos "0.1")

pico_generate_pio_header(Rtos ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Rtos 1)
pico_enable_stdio_usb(Rtos 0)

# Add the standard library to the build
target_link_libraries(Rtos
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(Rtos PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(Rtos 
        pico_cyw43_arch_none
        FreeRTOS-Kernel-Heap4
        pico_async_context_freertos
        hardware_adc
        hardware_pio
        hardware_clocks
        pico_stdlib
        pico_aon_timer
        hardware_clocks
        hardware_pwm
        )

pico_add_extra_outputs(Rtos)

