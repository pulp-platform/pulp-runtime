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


#ifndef __ARCHI_CHIPS_SAFETY_ISLAND_H__
#define __ARCHI_CHIPS_SAFETY_ISLAND_H__

#include "archi/chips/safety-island/properties.h"
#include "archi/chips/safety-island/safety_soc_ctrl.h"

// cv32e40p-specific
#include "archi/cv32e40p/cv32e40p.h"
#include "archi/riscv/priv_1_12.h"

#include "archi/chips/safety-island/memory_map.h"
#include "archi/stdout/stdout_v3.h"
#include "archi/bus_err_unit/bus_err_unit_v1.h"
#include "archi/hmr/hmr_v1.h"

#endif
