// Copyright 2022 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Nils Wistoff <nwistoff@iis.ee.ethz.ch>
// Paul Scheffler <paulsc@iis.ee.ethz.ch>

// Adapted from Cheshire

#include "host_uart.h"

static inline volatile uint8_t *reg8(void *base, int offs) {
    return (volatile uint8_t *)(base + offs);
}

static inline void fence() {
    asm volatile("fence" ::: "memory");
}

int uart_read_ready(void *uart_base) {
    return *reg8(uart_base, UART_LINE_STATUS_REG_OFFSET) & (1 << UART_LINE_STATUS_DATA_READY_BIT);
}

static inline int __uart_write_ready(void *uart_base) {
    return *reg8(uart_base, UART_LINE_STATUS_REG_OFFSET) & (1 << UART_LINE_STATUS_THR_EMPTY_BIT);
}

static inline int __uart_write_idle(void *uart_base) {
    return __uart_write_ready(uart_base) &&
           *reg8(uart_base, UART_LINE_STATUS_REG_OFFSET) & (1 << UART_LINE_STATUS_TMIT_EMPTY_BIT);
}

void uart_write(void *uart_base, uint8_t byte) {
    while (!__uart_write_ready(uart_base))
        ;
    *reg8(uart_base, UART_THR_REG_OFFSET) = byte;
}

void uart_write_flush(void *uart_base) {
    fence();
    while (!__uart_write_idle(uart_base))
        ;
}

uint8_t uart_read(void *uart_base) {
    while (!uart_read_ready(uart_base))
        ;
    return *reg8(uart_base, UART_RBR_REG_OFFSET);
}
