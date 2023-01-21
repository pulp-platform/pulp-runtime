// Generated register defines for timer

// Copyright information found in source file:
// Copyright ETH Zurich

// Licensing information found in source file:
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef _TIMER_REG_DEFS_
#define _TIMER_REG_DEFS_

#ifdef __cplusplus
extern "C" {
#endif
// Register width
#define TIMER_PARAM_REG_WIDTH 32

// Common Interrupt Offsets

// Timer low configuration
#define TIMER_CFG_LO(id) (TIMER##id##_BASE_ADDR + 0x0)
#define TIMER_CFG_LO_REG_OFFSET 0x0
#define TIMER_CFG_LO_EN 0
#define TIMER_CFG_LO_RST 1
#define TIMER_CFG_LO_IRQEN 2
#define TIMER_CFG_LO_MODE 4
#define TIMER_CFG_LO_ONES 5
#define TIMER_CFG_LO_PEN 6
#define TIMER_CFG_LO_CCFG 7
#define TIMER_CFG_LO_PVAL_MASK 0xff
#define TIMER_CFG_LO_PVAL_OFFSET 8
#define TIMER_CFG_LO_CASC 31

// Timer low counter value
#define TIMER_CNT_LO(id) (TIMER##id##_BASE_ADDR + 0x8)
#define TIMER_CNT_LO_REG_OFFSET 0x8

// Timer low comparator value
#define TIMER_CMP_LO(id) (TIMER##id##_BASE_ADDR + 0x10)
#define TIMER_CMP_LO_REG_OFFSET 0x10

// Start timer low counter
#define TIMER_START_LO(id) (TIMER##id##_BASE_ADDR + 0x18)
#define TIMER_START_LO_REG_OFFSET 0x18
#define TIMER_START_LO_STRT 0

// Reset timer low counter
#define TIMER_RESET_LO(id) (TIMER##id##_BASE_ADDR + 0x20)
#define TIMER_RESET_LO_REG_OFFSET 0x20
#define TIMER_RESET_LO_RST 0

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _TIMER_REG_DEFS_
// End generated register defines for timer
