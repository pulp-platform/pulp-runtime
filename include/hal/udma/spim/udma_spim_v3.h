/*
 * Copyright (C) 2018 ETH Zurich and University of Bologna
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

#ifndef __HAL_UDMA_UDMA_SPIM_V3_H__
#define __HAL_UDMA_UDMA_SPIM_V3_H__

#include "archi/udma/spim/udma_spim_v3.h"

#define UDMA_SPIM_OFFSET(id)          UDMA_PERIPH_OFFSET(ARCHI_UDMA_SPIM_ID(id))
#define UDMA_SPIM_CMD_ADDR(id)        (ARCHI_UDMA_ADDR + UDMA_SPIM_OFFSET(id) + 0x20)
#define UDMA_SPIM_RX_ADDR(id)         (ARCHI_UDMA_ADDR + UDMA_SPIM_OFFSET(id) + 0x00)
#define UDMA_SPIM_TX_ADDR(id)         (ARCHI_UDMA_ADDR + UDMA_SPIM_OFFSET(id) + 0x10)
#define UDMA_SPIM_CUSTOM_ADDR(id)     (ARCHI_UDMA_ADDR + UDMA_SPIM_OFFSET(id) + UDMA_CHANNEL_CUSTOM_OFFSET)

static inline unsigned int plp_spim_reg_read(int channel, unsigned int addr)
{ //adr is an offset, expected SPIM_STATUS_OFFSET or SPIM_SETUP_OFFSET
  return pulp_read32(ARCHI_UDMA_ADDR + UDMA_SPIM_OFFSET(channel) + UDMA_CHANNEL_CUSTOM_OFFSET + addr);
}

static inline void plp_spim_reg_write(int channel, unsigned int addr, unsigned int cfg)
{ //adr is an offset, expected SPIM_STATUS_OFFSET or SPIM_SETUP_OFFSET
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_SPIM_OFFSET(channel) + UDMA_CHANNEL_CUSTOM_OFFSET + addr, cfg);
}

//static inline void plp_qspi_setup(int channel, int parity, uint16_t clk_counter)
//{
//
//  // [31:16]: clock divider (from SoC clock)
//  // [9]: RX enable
//  // [8]: TX enable
//  // [3]: stop bits  0 = 1 stop bit
//  //                 1 = 2 stop bits
//  // [2:1]: bits     00 = 5 bits
//  //                 01 = 6 bits
//  //                 10 = 7 bits
//  //                 11 = 8 bits
//  // [0]: parity
//
//  unsigned int val = 0x0306 | parity; // both tx and rx enabled; 8N1 configuration; 1 stop bits
//
//  val |= ((clk_counter) << 16);
//
//  pulp_write32(ARCHI_UDMA_ADDR + UDMA_QSPI_OFFSET(channel) + UDMA_CHANNEL_CUSTOM_OFFSET + QSPI_SETUP_OFFSET, val);
//}

static inline void plp_qspi_disable(int channel) {
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_SPIM_OFFSET(channel) + UDMA_CHANNEL_CUSTOM_OFFSET + UART_SETUP_OFFSET, 0x00500006);
}

static inline int plp_qspi_tx_busy(int channel) {
  return pulp_read32(ARCHI_UDMA_ADDR + UDMA_SPIM_OFFSET(channel) + UDMA_CHANNEL_CUSTOM_OFFSET + UART_STATUS_OFFSET) & 1;
}

static inline int plp_qspi_rx_busy(int channel) {
  return (pulp_read32(ARCHI_UDMA_ADDR + UDMA_SPIM_OFFSET(channel) + UDMA_CHANNEL_CUSTOM_OFFSET + UART_STATUS_OFFSET) >> 1) & 1;
}

#endif
