/*
 * MIT License 
 * 
 * Copyright (c) 2020 Kirill Kotyagin
 */

#include <stm32f1xx.h>
#include "system_clock.h"
#include "system_interrupts.h"
#include "status_led.h"
#include "device_config.h"
#include "usb.h"
#include "usb_cdc.h"

int main() {
    system_clock_init();
    system_interrupts_init();
    device_config_init();
    status_led_init();
    
    /* Startup indication: 5 LED blinks */
    for (int i = 0; i < 5; i++) {
        status_led_set(1);
        for (volatile int delay = 0; delay < 200000; delay++) __NOP();
        status_led_set(0);
        for (volatile int delay = 0; delay < 200000; delay++) __NOP();
    }
    
    /* Additional delay before USB initialization */
    for (volatile int delay = 0; delay < 500000; delay++) __NOP();
    
    usb_init();
    while (1) {
        /* USB protocol events handled by interrupts, but CDC polling needed for data flow */
        usb_cdc_poll();
    }
}
