/*
 * Copyright (C) 2023 ETH Zurich, University of Bologna
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

#ifndef __HAL_BUS_ERR_UNIT_V1_H__
#define __HAL_BUS_ERR_UNIT_V1_H__

#include "archi/pulp.h"

static inline unsigned int data_bus_err_get_addr_32() {
  return pulp_read32(ARCHI_DATA_ERR_ADDR+BUS_ERR_UNIT_ERR_ADDR_REG_OFFSET);
}

static inline uint64_t data_bus_err_get_addr_64() {
  uint64_t tmp = pulp_read32(ARCHI_DATA_ERR_ADDR+BUS_ERR_UNIT_ERR_ADDR_TOP_REG_OFFSET);
  return (tmp << 32) | (uint64_t)(pulp_read32(ARCHI_DATA_ERR_ADDR+BUS_ERR_UNIT_ERR_ADDR_REG_OFFSET));
}

static inline unsigned int data_bus_err_get_err_and_pop() {
  return pulp_read32(ARCHI_DATA_ERR_ADDR+BUS_ERR_UNIT_ERR_CODE_REG_OFFSET);
}

#endif // __HAL_BUS_ERR_UNIT_V1_H__
