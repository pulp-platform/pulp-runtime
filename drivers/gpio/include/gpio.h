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

#include <stdint.h>

#define GPIO_DIRECTION_OUTPUT 1
#define GPIO_DIRECTION_INPUT  0


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
void gpio_configure(uint32_t number, uint32_t direction);

/**
 * Get the current value of the GPIO.
 *
 * The GPIO must have been configured as an input using configure_gpio().
 *
 * @param number The GPIO number to confgiure. You can either use directly an integer number or on of the PAD_GPIOXX macros.
 * @returns 0 if the gpio is currently at logic level LOW or 1 if it is at logic level HIGH.
 */
uint32_t gpio_get(uint32_t number);

/**
 * Drive the GPIO to the provided value.
 *
 * The corresponding GPIO must have been configured as an output using configure_gpio().
 *
 * @param number The GPIO number to confgiure. You can either use directly an integer number or on of the PAD_GPIOXX macros.
 * @param value Either 0 or 1.
 */
void gpio_set(uint32_t number, uint32_t value);

#endif /* GPIO_H */
