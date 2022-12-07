// Generic Hardware Abstraction Layer for the GPIO peripheral

#ifndef _GPIO_REG_DEFS_
#define _GPIO_REG_DEFS_

#ifdef __cplusplus
extern "C" {
#endif

// Info register that contains information about this peripheral.
#define GPIO_INFO_REG_OFFSET 0x0
#define GPIO_INFO_REG_GPIO_CNT_MASK 0x3ff
#define GPIO_INFO_REG_GPIO_CNT_OFFSET 0
#define GPIO_INFO_REG_GPIO_CNT_FIELD                                        \
  ((bitfield_field32_t) { .mask = GPIO_INFO_GPIO_CNT_MASK, .index = GPIO_INFO_GPIO_CNT_OFFSET })
#define GPIO_INFO_REG_VERSION_MASK 0x3ff
#define GPIO_INFO_REG_VERSION_OFFSET 10
#define GPIO_INFO_REG_VERSION_FIELD                                         \
  ((bitfield_field32_t) { .mask = GPIO_INFO_VERSION_MASK, .index = GPIO_INFO_VERSION_OFFSET })

// Global configuration register for the peripheral
#define GPIO_CFG_REG_OFFSET 0x4
#define GPIO_CFG_INTRPT_MODE_BIT 0

// Set the IO Mode of the GPIO. (common parameters)
#define GPIO_MODE_REG_OFFSET_BASE 0x8
#define GPIO_MODE_REG_VALUE_INPUT_ONLY 0x0
#define GPIO_MODE_REG_VALUE_OUTPUT_ACTIVE 0x1
#define GPIO_MODE_REG_VALUE_OPEN_DRAIN0 0x2
#define GPIO_MODE_REG_VALUE_OPEN_DRAIN1 0x3

// Enable sampling on the corresponding GPIO (common parameters)
#define GPIO_EN_INPUT_REG_OFFSET_BASE 0x80

// Read the current input values of all GPIOs.
#define GPIO_INPUT_REG_OFFSET_BASE 0x100

// Set the output value of the corresponding GPIOs.
#define GPIO_OUTPUT_REG_OFFSET_BASE 0x180

// For each asserted bit in this register, set the corresponding bit in the
// padout register.
#define GPIO_SET_REG_OFFSET_BASE 0x200

// For each asserted bit in this register, clear the corresponding bit in the
// padout register.
#define GPIO_CLEAR_REG_OFFSET_BASE 0X280

// For each asserted bit in this register, toggle the corresponding bit in
// the padout register.
#define GPIO_TOGGLE_REG_OFFSET_BASE 0X300

// Enable Interrupts on rising edges for the corresponding GPIO
#define GPIO_INTRPT_RISE_EN_REG_OFFSET_BASE 0x380

// Enable Interrupts on falling edges for the corresponding GPIO
#define GPIO_INTRPT_FALL_EN_REG_OFFSET_BASE 0x400

// Enable logic high level-sensitive Interrupts on the corresponding GPIO
#define GPIO_INTRPT_HIGH_EN_REG_OFFSET_BASE 0x480

// Enable logic low level-sensitive Interrupts on the corresponding GPIO
#define GPIO_INTRPT_LOW_EN_REG_OFFSET_BASE 0x500

// Asserted if there is any pending interrupts on corresponding GPIOs.
#define GPIO_INTRPT_STATUS_REG_OFFSET_BASE 0x580

// Asserted if there is a pending rise interrupts on corresponding GPIOs.
#define GPIO_INTRPT_RISE_STATUS_REG_OFFSET_BASE 0x600

// Asserted if there is any pending fall interrupts on corresponding GPIOs.
#define GPIO_INTRPT_FALL_STATUS_REG_OFFSET_BASE 0x680

// Asserted if there is any pending high-level interrupts on corresponding
// GPIOs.
#define GPIO_INTRPT_LVL_HIGH_STATUS_REG_OFFSET_BASE 0x700

// Asserted if there is any pending low-level interrupts on corresponding
// GPIOs.
#define GPIO_INTRPT_LVL_LOW_STATUS_REG_OFFSET_BASE 0x780

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _GPIO_REG_DEFS_
// End generated register defines for gpio
