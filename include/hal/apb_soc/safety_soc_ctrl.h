/*
 * Copyright (C) 2023 ETH Zurich and University of Bologna
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
 */

#ifndef __HAL_SAFETY_SOC_CTRL_H__
#define __HAL_SAFETY_SOC_CTRL_H__

// Compatability Layer for miniPULP

#include "archi/pulp.h"
#include "hal/pulp_io.h"
#define APB_SOC_STATUS_EOC_BIT  31

static inline void apb_soc_bootaddr_set(unsigned int value) {
  pulp_write32(ARCHI_SOC_CTRL_ADDR + SAFETY_SOC_CTRL_BOOTADDR_REG_OFFSET, value);
}

static inline unsigned int apb_soc_bootaddr_get() {
  return pulp_read32(ARCHI_SOC_CTRL_ADDR + SAFETY_SOC_CTRL_BOOTADDR_REG_OFFSET);
}

static inline void apb_soc_status_set(unsigned int value) {
  pulp_write32(ARCHI_SOC_CTRL_ADDR + SAFETY_SOC_CTRL_CORESTATUS_REG_OFFSET, value | (1<<APB_SOC_STATUS_EOC_BIT));
}



#endif // __HAL_APB_SOC_MINIPULP_SOC_CTRL_H__
