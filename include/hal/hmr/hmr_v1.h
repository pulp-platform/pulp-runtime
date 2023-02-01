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

#ifndef __HAL_HMR_HMR_V1_H__
#define __HAL_HMR_HMR_V1_H__

#include "archi/hmr/hmr_v1.h"
#include "archi/pulp.h"
#include <stdbool.h>

#define NUM_TMR_GROUPS (ARCHI_CLUSTER_NB_PE/3)
#define NUM_TMR_CORES  (ARCHI_CLUSTER_NB_PE-(ARCHI_CLUSTER_NB_PE%3))
#define NUM_DMR_GROUPS (ARCHI_CLUSTER_NB_PE/2)
#define NUM_DMR_CORES  (ARCHI_CLUSTER_NB_PE-(ARCHI_CLUSTER_NB_PE%2))

// Interleaved cores
#define TMR_IS_CORE(core_id)          (core_id<NUM_TMR_CORES)
#define DMR_IS_CORE(core_id)          (core_id<NUM_DMR_CORES)
#if HMR_IN_INTERLEAVED
#define TMR_GROUP_ID(core_id)         (core_id % NUM_TMR_GROUPS)
#define TMR_CORE_ID(group_id, offset) (group_id + (offset * NUM_TMR_GROUPS))
#define TMR_BARRIER_ID(group_id)      (1+group_id)
#define TMR_BARRIER_SETUP(group_id)   (1<<group_id | 1<<(group_id+NUM_TMR_GROUPS) | 1<<(group_id+2*NUM_TMR_GROUPS))
#define DMR_GROUP_ID(core_id)         (core_id % NUM_DMR_GROUPS)
#define DMR_CORE_ID(group_id, offset) (group_id + (offset * NUM_DMR_GROUPS))
#define DMR_BARRIER_ID(group_id)      (1+group_id)
#define DMR_BARRIER_SETUP(group_id)   (1<<group_id | 1<<(group_id+NUM_DMR_GROUPS))
#else
#define TMR_GROUP_ID(core_id)         (core_id/3)
#define TMR_CORE_ID(group_id, offset) ((group_id * 3) + core_offset)
#define TMR_BARRIER_ID(group_id)      (1+group_id+(group_id/2))
#define TMR_BARRIER_SETUP(group_id)   (1<<(3*group_id) | 1<<(3*group_id + 1) | 1<<(3*group_id+2))
#define DMR_GROUP_ID(core_id)         (core_id/2)
#define DMR_CORE_ID(group_id, offset) ((group_id * 2) + core_offset)
#define DMR_BARRIER_ID(group_id)      (1+group_id)
#define DMR_BARRIER_SETUP(group_id)   (1<<(2*group_id) | 1<<(2*group_id + 1))
#endif
#define TMR_IS_MAIN_CORE(core_id)     (TMR_IS_CORE(core_id) && (TMR_CORE_ID(TMR_GROUP_ID(core_id), 0) == core_id))
#define DMR_IS_MAIN_CORE(core_id)     (DMR_IS_CORE(core_id) && (DMR_CORE_ID(DMR_GROUP_ID(core_id), 0) == core_id))

void pos_hmr_tmr_irq();

static inline unsigned int hmr_get_available_config(unsigned int cid) {
  return pulp_read32(ARCHI_HMR_GLOBAL_ADDR(cid) + HMR_TOP_OFFSET + HMR_REGISTERS_AVAIL_CONFIG_REG_OFFSET);
}

static inline unsigned int hmr_get_active_cores(unsigned int cid) {
  return pulp_read32(ARCHI_HMR_GLOBAL_ADDR(cid) + HMR_TOP_OFFSET + HMR_REGISTERS_CORES_EN_REG_OFFSET);
}

static inline unsigned int hmr_get_core_status(unsigned int cid, unsigned int core_id) {
  return pulp_read32(ARCHI_HMR_GLOBAL_ADDR(cid) + HMR_CORE_OFFSET + (core_id * HMR_CORE_INCREMENT) + HMR_CORE_REGS_CURRENT_MODE_REG_OFFSET);
}

static inline unsigned int hmr_get_core_mismatches(unsigned int cid, unsigned int core_id) {
  return pulp_read32(ARCHI_HMR_GLOBAL_ADDR(cid) + HMR_CORE_OFFSET + (core_id * HMR_CORE_INCREMENT) + HMR_CORE_REGS_MISMATCHES_REG_OFFSET);
}

static inline unsigned int hmr_reset_core_mismatches(unsigned int cid, unsigned int core_id) {
  return pulp_write32(ARCHI_HMR_GLOBAL_ADDR(cid) + HMR_CORE_OFFSET + (core_id * HMR_CORE_INCREMENT) + HMR_CORE_REGS_MISMATCHES_REG_OFFSET, 0);
}

static inline unsigned int hmr_get_dmr_status_all(unsigned int cid) {
  return pulp_read32(ARCHI_HMR_GLOBAL_ADDR(cid) + HMR_TOP_OFFSET + HMR_REGISTERS_DMR_ENABLE_REG_OFFSET);
}

static inline void hmr_set_dmr_status_all(unsigned int cid, unsigned int status) {
  pulp_write32(ARCHI_HMR_GLOBAL_ADDR(cid) + HMR_TOP_OFFSET + HMR_REGISTERS_DMR_ENABLE_REG_OFFSET, status);
}

static inline void hmr_enable_all_dmr(unsigned int cid) {
  hmr_set_dmr_status_all(cid, (1<<NUM_DMR_GROUPS)-1);
}

static inline void hmr_disable_all_dmr(unsigned int cid) {
  hmr_set_dmr_status_all(cid, 0);
}

static inline void hmr_set_dmr_config(unsigned int cid, unsigned int dmr_id, bool rapid_recovery) {
  pulp_write32(ARCHI_HMR_GLOBAL_ADDR(cid) + HMR_DMR_OFFSET + HMR_DMR_INCREMENT*dmr_id + HMR_DMR_REGS_DMR_CONFIG_REG_OFFSET,
    (rapid_recovery ? 1<<HMR_DMR_REGS_DMR_CONFIG_RAPID_RECOVERY_BIT : 0));
}

static inline void hmr_set_dmr_config_all(unsigned int cid, bool rapid_recovery) {
  pulp_write32(ARCHI_HMR_GLOBAL_ADDR(cid) + HMR_TOP_OFFSET + HMR_REGISTERS_DMR_CONFIG_REG_OFFSET,
    (rapid_recovery ? 1<<HMR_REGISTERS_DMR_CONFIG_RAPID_RECOVERY_BIT : 0));
}

static inline unsigned int hmr_get_tmr_status_all(unsigned int cid) {
  return pulp_read32(ARCHI_HMR_GLOBAL_ADDR(cid) + HMR_TOP_OFFSET + HMR_REGISTERS_TMR_ENABLE_REG_OFFSET);
}

static inline void hmr_set_tmr_status_all(unsigned int cid, unsigned int status) {
  pulp_write32(ARCHI_HMR_GLOBAL_ADDR(cid) + HMR_TOP_OFFSET + HMR_REGISTERS_TMR_ENABLE_REG_OFFSET, status);
}

static inline void hmr_enable_all_tmr(unsigned int cid) {
  hmr_set_tmr_status_all(cid, (1<<NUM_TMR_GROUPS)-1);
}

static inline void hmr_disable_all_tmr(unsigned int cid) {
  hmr_set_tmr_status_all(cid, 0);
}

static inline void hmr_set_tmr_config(unsigned int cid, unsigned int tmr_id, bool delay_resynch, bool setback, bool reload_setback, bool rapid_recovery) {
  pulp_write32(ARCHI_HMR_GLOBAL_ADDR(0) + HMR_TMR_OFFSET + HMR_TMR_INCREMENT*tmr_id + HMR_TMR_REGS_TMR_CONFIG_REG_OFFSET,
    (delay_resynch  ? 1<<HMR_TMR_REGS_TMR_CONFIG_DELAY_RESYNCH_BIT  : 0) |
    (setback        ? 1<<HMR_TMR_REGS_TMR_CONFIG_SETBACK_BIT        : 0) |
    (reload_setback ? 1<<HMR_TMR_REGS_TMR_CONFIG_RELOAD_SETBACK_BIT : 0) |
    (rapid_recovery ? 1<<HMR_TMR_REGS_TMR_CONFIG_RAPID_RECOVERY_BIT : 0));
}

static inline void hmr_set_tmr_config_all(unsigned int cid, bool delay_resynch, bool setback, bool reload_setback, bool rapid_recovery) {
  pulp_write32(ARCHI_HMR_GLOBAL_ADDR(0) + HMR_TOP_OFFSET + HMR_REGISTERS_TMR_CONFIG_REG_OFFSET,
    (delay_resynch  ? 1<<HMR_REGISTERS_TMR_CONFIG_DELAY_RESYNCH_BIT  : 0) |
    (setback        ? 1<<HMR_REGISTERS_TMR_CONFIG_SETBACK_BIT        : 0) |
    (reload_setback ? 1<<HMR_REGISTERS_TMR_CONFIG_RELOAD_SETBACK_BIT : 0) |
    (rapid_recovery ? 1<<HMR_REGISTERS_TMR_CONFIG_RAPID_RECOVERY_BIT : 0));
}

static void hmr_tmr_barrier_setup_all() {
  for (int i = 0; i < NUM_TMR_GROUPS; i++) {
    eu_bar_setup(eu_bar_addr(TMR_BARRIER_ID(i)), TMR_BARRIER_SETUP(i));
  }
}

#endif // __HAL_HMR_HMR_V1_H__
