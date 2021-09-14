/*
 * Copyright (C) 2021 ETH Zurich and University of Bologna
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

#ifndef __HAL_TCLS_H
#define __HAL_TCLS_H

#include "archi/pulp.h"
#include <stdio.h>

static inline void enable_tcls() {
  int * tcls_config;
  for (int i = 0; i < 2; i++) {
    tcls_config = (int*)(ARCHI_CLUSTER_GLOBAL_ADDR(0) + ARCHI_TCLS_ADDR + 0x4 + i*0x100);
    *tcls_config = 1;
  }
  printf("In TCLS config\n");
}

static inline void disable_tcls() {
  int * tcls_config;
  for (int i = 0; i < 2; i++) {
    tcls_config = (int*)(ARCHI_CLUSTER_GLOBAL_ADDR(0) + ARCHI_TCLS_ADDR + 0x4 + i*0x100);
    *tcls_config = 0;
  }
  printf("TCLS disabled\n");
}

static inline int check_tcls_enabled() {
  int * tcls_config;
  tcls_config = (int*)(ARCHI_CLUSTER_GLOBAL_ADDR(0) + ARCHI_TCLS_ADDR + 0x4);
  return *tcls_config > 0;
}

#endif // __HAL_TCLS_H
