/*
 * Copyright (C) 2018 ETH Zurich, University of Bologna
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


#ifndef __ARCHI_CHIPS_KAIROS_H__
#define __ARCHI_CHIPS_KAIROS_H__

#include "archi/chips/kairos/properties.h"
#include "archi/chips/kairos/apb_soc_ctrl.h"

#include "archi/itc/itc_v1.h"

// cv32e40p-specific
#include "archi/cv32e40p/cv32e40p.h"
#include "archi/riscv/priv_1_11.h"

#include "archi/chips/kairos/memory_map.h"
#include "archi/chips/kairos/apb_soc.h"
#include "archi/stdout/stdout_v3.h"
#include "archi/dma/mchan_v7.h"

#include "archi/udma/spim/udma_spim_v3.h"
#include "archi/udma/i2c/udma_i2c_v2.h"
#include "archi/udma/uart/udma_uart_v1.h"
#include "archi/udma/udma_v3.h"

#endif
