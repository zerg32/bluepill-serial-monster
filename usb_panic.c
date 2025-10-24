/*
 * MIT License 
 * 
 * Copyright (c) 2020 Kirill Kotyagin
 */

#include <stm32f1xx.h>
#include "status_led.h"
#include "usb_panic.h"
#include "device_config.h"
#include "gpio.h"

static void usb_panic_light_led_direct() {
    /* Try to use device config LED pin if available, otherwise fall back to PB1 */
    const device_config_t *cfg = device_config_get();
    const gpio_pin_t *led_pin = NULL;
    if (cfg) {
        led_pin = &cfg->status_led_pin;
    }
    if (led_pin && led_pin->port) {
        /* Init pin and set it */
        gpio_pin_init(led_pin);
        gpio_pin_set(led_pin, 1);
    } else {
        /* Fallback: set PB1 (Maple Mini LED) as push-pull output and drive it high */
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
        /* Configure PB1 as general purpose push-pull output, low speed */
        volatile uint32_t *crx = &GPIOB->CRL + (1 >> 3);
        uint8_t crx_offset = (1 & 0x07) << 2;
        uint32_t modecfg = 0;
        /* mode: output 10 MHz (01), CNF: general purpose push-pull (00) */
        modecfg |= GPIO_CRL_MODE0_1;
        *crx &= ~((GPIO_CRL_CNF0 | GPIO_CRL_MODE0) << crx_offset);
        *crx |= (modecfg << crx_offset);
        GPIOB->BSRR = GPIO_BSRR_BS0 << 1; /* set PB1 */
    }
}

void usb_panic() {
    __disable_irq();
    /* Best-effort light the LED even with interrupts disabled */
    usb_panic_light_led_direct();
    while (1) {
        __NOP();
    }
}
