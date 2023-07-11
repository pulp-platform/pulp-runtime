/*
 * Copyright (C) 2019 ETH Zurich, University of Bologna
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

#include "pulp.h"

void pos_soc_init()
{

  pos_freq_domains[PI_FREQ_DOMAIN_FC] = ARCHI_FPGA_SOC_FREQUENCY;

  pos_freq_domains[PI_FREQ_DOMAIN_PERIPH] = ARCHI_FPGA_PER_FREQUENCY;

  pos_freq_domains[PI_FREQ_DOMAIN_CL] = ARCHI_FPGA_CL_FREQUENCY;

}

