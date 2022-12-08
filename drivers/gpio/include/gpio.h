/*
 * Copyright (C) 2022 ETH Zurich, University of Bologna
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Title: gpio.h<include>
 * Author: Manuel Eggimann  <meggimann@iis.ee.ethz.ch>
 * Date: 09.03.2022
 *
 * Description: Driver for the PULP GPIO peripheral
 */
#ifndef GPIO_H
#define GPIO_H
#ifndef LANGUAGE_ASSEMBLY

#include <stdint.h>
#include "common/bitfield.h"
#include "gpio_hal.h"
#include "hal/utils.h"
#include "archi/utils.h"


#define GPIO_DIRECTION_OUTPUT 1
#define GPIO_DIRECTION_INPUT  0


typedef struct {
  uint32_t base_addr;
} gpios_t;

/**
 * Initializes the peripheral handle structure
 *
 * This function must be called before using the handle for gpio interaction.
 *
 * @param hanlde A pointer to the handle to initialize.
 */
void gpio_init(gpios_t* handle);


/**
 * Configure the provided GPIO as either an input or output.
 *
 * It is the user's responsibility to ensure that the GPIO to be configures is
 * actually exposed to its corresponding IO pad. I.e. if your SoC contains an IO
 * multiplexer you have to make sure you configure it apropriately to expose the
 * GPIO on the pad.
 *
 * @param number The GPIO number to confgiure. You can either use directly an integer number or on of the PAD_GPIOXX macros.
 * @param direction Either GPIO_DIRECTION_INPUT (0) or GPIO_DIRECTION_OUTPUT
 */
void gpio_configure(gpios_t* handle, uint32_t number, uint32_t direction);

/**
 * Get the current value of the GPIO.
 *
 * The GPIO must have been configured as an input using configure_gpio().
 *
 * @param number The GPIO number to confgiure. You can either use directly an integer number or on of the PAD_GPIOXX macros.
 * @returns 0 if the gpio is currently at logic level LOW or 1 if it is at logic level HIGH.
 */
static inline uint32_t gpio_get(gpios_t* handle, uint32_t number) {
  uint32_t addr = handle->base_addr + GPIO_INPUT_REG_OFFSET_BASE + number/32*4;
  uint32_t reg = archi_read32(addr);
  return bitfield_bit32_read(reg, number%32);
  hal_compiler_barrier();
}

/**
 * Drive the GPIO to the provided value.
 *
 * The corresponding GPIO must have been configured as an output using configure_gpio().
 *
 * @param number The GPIO number to confgiure. You can either use directly an integer number or on of the PAD_GPIOXX macros.
 * @param value Either 0 or 1.
 */
static inline void gpio_set(gpios_t* handle, uint32_t number, uint32_t value) {
  uint32_t addr;
  if (value == 1) {
    addr = handle->base_addr + GPIO_SET_REG_OFFSET_BASE + number/32*4;
  } else {
    addr = handle->base_addr + GPIO_CLEAR_REG_OFFSET_BASE + number/32*4;
  }
  archi_write32(addr, 1<<(number%32));
  hal_compiler_barrier();
}


/**
 * Toggle the GPIO
 *
 * The corresponding GPIO must have been configured as an output using configure_gpio().
 *
 * @param number The GPIO number to confgiure. You can either use directly an integer number or on of the PAD_GPIOXX macros.
 */
static inline void gpio_toggle(gpios_t* handle, uint32_t number) {
  uint32_t addr = handle->base_addr + GPIO_TOGGLE_REG_OFFSET_BASE + number/32*4;
  archi_write32(addr, 1<<(number%32));
  hal_compiler_barrier();
}


#endif
#endif /* GPIO_H */
