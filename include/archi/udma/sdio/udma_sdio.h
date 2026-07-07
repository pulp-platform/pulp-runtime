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

#ifndef __ARCHI_UDMA_UDMA_SDIO_H__
#define __ARCHI_UDMA_UDMA_SDIO_H__

/* -------------------------------------------------------------------------
   -- SDIO CMDs
   -------------------------------------------------------------------------
   See page 49 (60 pdf numbering) of Physical Layer Simplified Spec Ver2.00
*/

#define CMD0   0x000 // GO_IDLE_STATE
#define CMD2   0x200 // ALL_SEND_CID
#define CMD3   0x300 // SEND_RELATIVE_ADDR
#define ACMD6  0x600
#define CMD7   0x700
#define CMD8   0x800
#define CMD55  0x3700
#define ACMD41 0x2900 // ACMD, need to be used after APP_SPEC_CMD
#define CMD8   0x800
#define CMD9   0x900
#define CMD10  0x0A00
#define CMD11  0x0B00
#define CMD12  0x0C00
#define CMD13  0x0D00
#define CMD16  0x1000
#define CMD17  0x1100 // READ_SINGLE_BLOCK
#define CMD18  0x1200
#define CMD24  0x1800 // WRITE_SINGLE_BLOCK
#define CMD25  0x1900 // WRITE_MULTIPLE_BLOCK

//UDMA_SDIO_CMD_RESP_TYPE
#define RSP_48_CRC     0x1
#define RSP_48_NO_CRC  0x2
#define RSP_136        0x3
#define RSP_48_BUSY    0x4

//SDIO_CMD_ERR_STATUS TYPES
#define UDMA_SDIO_NO_ERR_CMD 		 0
#define UDMA_SDIO_RESP_TIME_OUT_CMD  1
#define UDMA_SDIO_RESP_WRONG_DIR_CMD 2
#define UDMA_SDIO_RESP_BUSY_TOUT_CMD 3

//SDIO_DATA_ERR_STATUS TYPES
#define UDMA_SDIO_NO_ERR_DATA 		 0
#define UDMA_SDIO_RESP_TIME_OUT_DATA 1

//UDMA_SDIO_CMD_OP field
#define UDMA_SDIO_CMD_RESP_TYPE_OFFSET 0
#define UDMA_SDIO_CMD_OP_OFFSET 8

//UDMA_SDIO_CMD_ARG field
#define UDMA_SDIO_CMD_ARG_OFFSET 0

//UDMA_SDIO_DATA_SETUP field
#define UDMA_SDIO_DATA_EN_OFFSET   0
#define UDMA_SDIO_DATA_RWN_OFFSET  1
#define UDMA_SDIO_DATA_QUAD_OFFSET 2
#define UDMA_SDIO_BLOCK_NUM_OFFSET 8
#define UDMA_SDIO_BLOCK_SIZE_OFFSET 16

//UDMA_SDIO_STATUS field
#define UDMA_SDIO_EOT_OFFSET 0
#define UDMA_SDIO_ERROR_OFFSET 1
#define UDMA_SDIO_CMD_ERR_STATUS_OFFSET 16
#define UDMA_SDIO_DATA_ERR_STATUS_OFFSET 24

//UDMA_SDIO_STOPCMD_OP field
#define UDMA_SDIO_STOPCMD_RSP_TYPE_OFFSET 0
#define UDMA_SDIO_STOPCMD_OPTYPE_OFFSET 8

//SDIO registers
#define UDMA_SDIO_OFFSET(id)			UDMA_PERIPH_OFFSET(ARCHI_UDMA_SDIO_ID(id))

#define UDMA_SDIO_RX_ADDR(id)			(ARCHI_UDMA_ADDR + UDMA_SDIO_OFFSET(id) + UDMA_CHANNEL_RX_OFFSET)
#define UDMA_SDIO_RX_SIZE(id)       (UDMA_SDIO_RX_ADDR(id) + UDMA_CHANNEL_SIZE_OFFSET)
#define UDMA_SDIO_RX_CFG(id)        (UDMA_SDIO_RX_ADDR(id) + UDMA_CHANNEL_CFG_OFFSET)

#define UDMA_SDIO_TX_ADDR(id)       (ARCHI_UDMA_ADDR + UDMA_SDIO_OFFSET(id) + UDMA_CHANNEL_TX_OFFSET)
#define UDMA_SDIO_TX_SIZE(id)       (UDMA_SDIO_TX_ADDR(id) + UDMA_CHANNEL_SIZE_OFFSET)
#define UDMA_SDIO_TX_CFG(id)        (UDMA_SDIO_TX_ADDR(id) + UDMA_CHANNEL_CFG_OFFSET)

#define UDMA_SDIO_CMD_OP(id)        (ARCHI_UDMA_ADDR + UDMA_SDIO_OFFSET(id) + 0x20) // R/W
#define UDMA_SDIO_CMD_ARG(id)       (ARCHI_UDMA_ADDR + UDMA_SDIO_OFFSET(id) + 0x24) // R/W
#define UDMA_SDIO_DATA_SETUP(id)    (ARCHI_UDMA_ADDR + UDMA_SDIO_OFFSET(id) + 0x28) // W

#define UDMA_SDIO_START(id)         (ARCHI_UDMA_ADDR + UDMA_SDIO_OFFSET(id) + 0x2C) // W
#define UDMA_SDIO_RSP0(id)         	(ARCHI_UDMA_ADDR + UDMA_SDIO_OFFSET(id) + 0x30) // R
#define UDMA_SDIO_RSP1(id)         	(ARCHI_UDMA_ADDR + UDMA_SDIO_OFFSET(id) + 0x34) // R
#define UDMA_SDIO_RSP2(id)         	(ARCHI_UDMA_ADDR + UDMA_SDIO_OFFSET(id) + 0x38) // R
#define UDMA_SDIO_RSP3(id)         	(ARCHI_UDMA_ADDR + UDMA_SDIO_OFFSET(id) + 0x3C) // R

#define UDMA_SDIO_CLK_DIV(id)       (ARCHI_UDMA_ADDR + UDMA_SDIO_OFFSET(id) + 0x40) // R/W
#define UDMA_SDIO_STATUS(id)        (ARCHI_UDMA_ADDR + UDMA_SDIO_OFFSET(id) + 0x44) // R/W
#define UDMA_SDIO_STOPCMD_OP(id)    (ARCHI_UDMA_ADDR + UDMA_SDIO_OFFSET(id) + 0x48) // R/W
#define UDMA_SDIO_STOPCMD_ARG(id)   (ARCHI_UDMA_ADDR + UDMA_SDIO_OFFSET(id) + 0x52) // R/W

#endif