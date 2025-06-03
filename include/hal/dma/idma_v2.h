/*
 * Copyright (C) 2025 ETH Zurich and University of Bologna
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

#ifndef __HAL_IDMA_V2_H__
#define __HAL_IDMA_V2_H__

#include <archi/dma/idma_v2.h>
#include "hal/pulp.h"

#define PLP_DMA_LOC2EXT 0
#define PLP_DMA_EXT2LOC 1

#define PLP_DMA_1D 0
#define PLP_DMA_2D 1

#define IDMA_EVENT 8 // all iDMA tx_cplt events are broadcast
#define IDMA_ID_COUNTER_WIDTH 32
#define IDMA_ID_MASK 0xffffffff

typedef enum {
  IDMA_PROT_AXI = 0, // AXI protocol: L2 memory
  IDMA_PROT_OBI = 1, // OBI protocol: L1 memory
  IDMA_PROT_INIT = 4 // INIT protocol: /dev/null (write to here and the stream disappears, read from here and get all-zeros)
} idma_prot_t;

typedef unsigned int dma_ext_t;

#define IDMA_DEFAULT_CONFIG 0x0
#define IDMA_DEFAULT_CONFIG_L1TOL2 (IDMA_DEFAULT_CONFIG | (IDMA_PROT_OBI << IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET) | (IDMA_PROT_AXI << IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET))
#define IDMA_DEFAULT_CONFIG_L2TOL1 (IDMA_DEFAULT_CONFIG | (IDMA_PROT_AXI << IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET) | (IDMA_PROT_OBI << IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET))
#define IDMA_DEFAULT_CONFIG_L1TOL1 (IDMA_DEFAULT_CONFIG | (IDMA_PROT_OBI << IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET) | (IDMA_PROT_OBI << IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET))

#define IDMA_DEFAULT_CONFIG_2D 0x400 // 0x1 << 10 = 0x400
#define IDMA_DEFAULT_CONFIG_L1TOL2_2D (IDMA_DEFAULT_CONFIG_2D | (IDMA_PROT_OBI << IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET) | (IDMA_PROT_AXI << IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET))
#define IDMA_DEFAULT_CONFIG_L2TOL1_2D (IDMA_DEFAULT_CONFIG_2D | (IDMA_PROT_AXI << IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET) | (IDMA_PROT_OBI << IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET))
#define IDMA_DEFAULT_CONFIG_L1TOL1_2D (IDMA_DEFAULT_CONFIG_2D | (IDMA_PROT_OBI << IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET) | (IDMA_PROT_OBI << IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET))

// TODO add 3D support
#define IDMA_DEFAULT_CONFIG_3D 0x800 // 0x1 << 11 = 0x800
#define IDMA_DEFAULT_CONFIG_L1TOL2_3D (IDMA_DEFAULT_CONFIG_3D | (IDMA_PROT_OBI << IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET) | (IDMA_PROT_AXI << IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET))
#define IDMA_DEFAULT_CONFIG_L2TOL1_3D (IDMA_DEFAULT_CONFIG_3D | (IDMA_PROT_AXI << IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET) | (IDMA_PROT_OBI << IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET))
#define IDMA_DEFAULT_CONFIG_L1TOL1_3D (IDMA_DEFAULT_CONFIG_3D | (IDMA_PROT_OBI << IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET) | (IDMA_PROT_OBI << IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET))

/** @name High-level DMA memory copy functions; compatible with MCHAN driver interface
 * The following functions can be used to trigger DMA transfers to copy data between the cluster memory (L1) and another memory outside the cluster (another cluster L1 or L2).
 * The DMA supports the following features:
 *   - Transfers are event-based. With event-based transfers the core can call a wait function to block execution until the transfer is done.
 *   - The DMA supports 2D and 3D transfers which allows transfering a 2D tile in one command. Additional information must then be given to specify the width of the tile and the number of bytes between 2 lines of the tile.
 *   - The event sent at the end of the transfer is broadcasted to all cluster cores.
 *   - To identify specific transfers, the DMA provides a transfer identifier.
 *   - Multiple transfers can be launched simultaneously, with them being executed 2-4 in parallel, with more waiting in a queue.
 */
/**@{*/


/** Memory transfer with event-based completion.
 *
  \param   ext     Address in the external memory where to access the data. There is no restriction on memory alignment.
  \param   loc     Address in the cluster memory where to access the data. There is no restriction on memory alignment.
  \param   size    Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \param   ext2loc If 1, the transfer is loading data from external memory and storing to cluster memory. If 0, it is the contrary
  \return       The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int plp_dma_memcpy(dma_ext_t ext, unsigned int loc, unsigned short size, int ext2loc);
static inline int plp_cl_dma_memcpy(dma_ext_t ext, unsigned int loc, unsigned short size, int ext2loc);


/** Cluster memory to external memory transfer with event-based completion.
 *
  \param   ext  Address in the external memory where to store the data. There is no restriction on memory alignment.
  \param   loc  Address in the cluster memory where to load the data. There is no restriction on memory alignment.
  \param   size Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \return       The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int plp_dma_l1ToExt(dma_ext_t ext, unsigned int loc, unsigned short size);
static inline int plp_cl_dma_l1ToExt(dma_ext_t ext, unsigned int loc, unsigned short size);


/** External memory to cluster memory transfer with event-based completion.
 *
  \param   loc  Address in the cluster memory where to store the data. There is no restriction on memory alignment.
  \param   ext  Address in the external memory where to load the data. There is no restriction on memory alignment.
  \param   size Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \return       The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int plp_dma_extToL1(unsigned int loc, dma_ext_t ext, unsigned short size);


/** 2-dimensional memory transfer with event-based completion.
 *
  \param   ext    Address in the external memory where to access the data. There is no restriction on memory alignment.
  \param   loc    Address in the cluster memory where to access the data. There is no restriction on memory alignment.
  \param   size   Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \param   stride 2D stride, which is the number of bytes which are added to the beginning of the current line to switch to the next one. Must fit 16 bits, i.e. must be inferior to 65536.
  \param   length 2D length, which is the number of transfered bytes after which the DMA will switch to the next line. Must fit 16 bits, i.e. must be inferior to 65536.
  \param   ext2loc If 1, the transfer is loading data from external memory and storing to cluster memory. If 0, it is the contrary
  \return         The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int plp_dma_memcpy_2d(dma_ext_t ext, unsigned int loc, unsigned int size, unsigned int stride, unsigned int length, int ext2loc);
static inline int plp_cl_dma_memcpy_2d(dma_ext_t ext, unsigned int loc, unsigned int size, unsigned int stride, unsigned int length, int ext2loc);


/** Cluster memory to external memory 2-dimensional transfer with event-based completion.
 *
  \param   ext    Address in the external memory where to store the data. There is no restriction on memory alignment.
  \param   loc    Address in the cluster memory where to load the data. There is no restriction on memory alignment.
  \param   size   Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \param   stride 2D stride, which is the number of bytes which are added to the beginning of the current line to switch to the next one. Must fit 16 bits, i.e. must be inferior to 65536. This applies only to the external memory.
  \param   length 2D length, which is the number of transfered bytes after which the DMA will switch to the next line. Must fit 16 bits, i.e. must be inferior to 65536. This applies only to the external memory.
  \return         The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int plp_dma_l1ToExt_2d(dma_ext_t ext, unsigned int loc, unsigned short size, unsigned short stride, unsigned short length);
static inline int plp_cl_dma_l1ToExt_2d(dma_ext_t ext, unsigned int loc, unsigned short size, unsigned short stride, unsigned short length);

/** External memory to cluster memory 2-dimensional transfer with event-based completion.
 *
  \param   loc    Address in the cluster memory where to store the data. There is no restriction on memory alignment.
  \param   ext    Address in the external memory where to load the data. There is no restriction on memory alignment.
  \param   size   Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \param   stride 2D stride, which is the number of bytes which are added to the beginning of the current line to switch to the next one. Must fit 16 bits, i.e. must be inferior to 65536. This applies only to the external memory.
  \param   length 2D length, which is the number of transfered bytes after which the DMA will switch to the next line. Must fit 16 bits, i.e. must be inferior to 65536. This applies only to the external memory.
  \return         The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer
*/
static inline int plp_dma_extToL1_2d(unsigned int loc, dma_ext_t ext, unsigned short size, unsigned short stride, unsigned short length);
static inline int plp_cl_dma_extToL1_2d(unsigned int loc, dma_ext_t ext, unsigned short size, unsigned short stride, unsigned short length);
//!@}


/** @name iDMA specific copy functions; these are preferred when writing new code.
 /**@{*/

/**
/** Arbitrary memory transfer with event-based completion.
 *
  \param   src       Address from where to copy data. There is no restriction on memory alignment.
  \param   dst       Address to which to copy data. There is no restriction on memory alignment.
  \param   size      Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be smaller than 65536.
  \param   src_prot  choose IDMA_PROT_AXI for transfer from L2, IDMA_PROT_OBI for transfer from L1 and IDMA_PROT_INIT for a transfer
                     of all-0 data
  \param   dst_prot  choose IDMA_PROT_AXI for transfer to L2, IDMA_PROT_OBI for transfer to L1 and IDMA_PROT_INIT for a transfer
                     to /dev/null (i.e. the stream will be "eaten"). Note that AXI-to-AXI transfers are not supported.
  \return          The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int pulp_idma_memcpy(unsigned int src, unsigned int dst, unsigned int size, idma_prot_t src_prot, idma_prot_t dst_prot);
static inline int pulp_cl_idma_memcpy(unsigned int src, unsigned int dst, unsigned int size, idma_prot_t src_prot, idma_prot_t dst_prot);

/** Cluster memory to external memory transfer with event-based completion.
 *
  \param   src  Address in the external memory where to store the data. There is no restriction on memory alignment.
  \param   dst  Address in the cluster memory where to load the data. There is no restriction on memory alignment.
  \param   size Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \return       The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
*/
static inline int pulp_idma_L1ToL2(unsigned int src, unsigned int dst, unsigned short size);
static inline int pulp_cl_idma_L1ToL2(unsigned int src, unsigned int dst, unsigned short size);

/** External memory to cluster memory transfer with event-based completion.
 *
  \param   src  Address in the cluster memory where to store the data. There is no restriction on memory alignment.
  \param   dst  Address in the external memory where to load the data. There is no restriction on memory alignment.
  \param   size Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \return       The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
*/
static inline int pulp_idma_L2ToL1(unsigned int src, unsigned int dst, unsigned short size);
static inline int pulp_cl_idma_L2ToL1(unsigned int src, unsigned int dst, unsigned short size);


/** Intra-cluster memory transfer with event-based completion.
 *
 \param   src  Address in the cluster memory where to store the data. There is no restriction on memory alignment.
 \param   dst  Address in the cluster memory where to load the data. There is no restriction on memory alignment.
 \param   size Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
 \return       The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
*/
static inline int pulp_idma_L1ToL1(unsigned int src, unsigned int dst, unsigned short size);
static inline int pulp_cl_idma_L1ToL1(unsigned int src, unsigned int dst, unsigned short size);

/** 2-dimensional memory transfer with event-based completion.
 *
  \param   src        Address from where to access the data. There is no restriction on memory alignment.
  \param   dst        Address where to write the data. There is no restriction on memory alignment.
  \param   size       Number of bytes per 1D transfer to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \param   src_stride 2D stride, which is the number of bytes which are added to the beginning of the current line to switch to the next one. Must fit 16 bits, i.e. must be inferior to 65536. Set equal to size to obtain an 1D transfer from the source region.
  \param   dst_stride 2D stride, which is the number of bytes which are added to the beginning of the current line to switch to the next one. Must fit 16 bits, i.e. must be inferior to 65536. Set equal to size to obtain an 1D transfer to the destination memory.
  \param   num_reps   Number of 1D transfers that comprise the 2D transfer.
  \param   src_prot   choose IDMA_PROT_AXI for transfer from L2, IDMA_PROT_OBI for transfer from L1 and IDMA_PROT_INIT for a transfer
  of all-0 data
  \param   dst_prot   choose IDMA_PROT_AXI for transfer to L2, IDMA_PROT_OBI for transfer to L1 and IDMA_PROT_INIT for a transfer
  to /dev/null (i.e. the stream will be "eaten"). Note that AXI-to-AXI transfers are not supported.
  \return             The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int pulp_idma_memcpy_2d(unsigned int src, unsigned int dst, unsigned int size, unsigned int src_stride, unsigned int dst_stride, unsigned int num_reps, idma_prot_t src_prot, idma_prot_t dst_prot);
static inline int pulp_cl_idma_memcpy_2d(unsigned int src, unsigned int dst, unsigned int size, unsigned int src_stride, unsigned int dst_stride, unsigned int num_reps, idma_prot_t src_prot, idma_prot_t dst_prot);

/** Cluster memory to external memory 2-dimensional transfer with event-based completion.
 *
  \param   src    Address in the external memory where to store the data. There is no restriction on memory alignment.
  \param   dst    Address in the cluster memory where to load the data. There is no restriction on memory alignment.
  \param   size   Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \param   src_stride 2D stride, which is the number of bytes which are added to the beginning of the current line to switch to the next one. Must fit 16 bits, i.e. must be inferior to 65536.
  \param   dst_stride 2D stride, which is the number of bytes which are added to the beginning of the current line to switch to the next one. Must fit 16 bits, i.e. must be inferior to 65536.
  \param   num_reps   Number of 1D transfers that comprise the 2D transfer.

  \return         The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int pulp_idma_L1ToL2_2d(unsigned int src, unsigned int dst, unsigned short size, unsigned int src_stride, unsigned int dst_stride, unsigned int num_reps);
static inline int pulp_cl_idma_L1ToL2_2d(unsigned int src, unsigned int dst, unsigned short size, unsigned int src_stride, unsigned int dst_stride, unsigned int num_reps);


/** Cluster memory to external memory 2-dimensional transfer with event-based completion.
 *
  \param   src    Address in the external memory where to store the data. There is no restriction on memory alignment.
  \param   dst    Address in the cluster memory where to load the data. There is no restriction on memory alignment.
  \param   size   Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \param   src_stride 2D stride, which is the number of bytes which are added to the beginning of the current line to switch to the next one. Must fit 16 bits, i.e. must be inferior to 65536.
  \param   dst_stride 2D stride, which is the number of bytes which are added to the beginning of the current line to switch to the next one. Must fit 16 bits, i.e. must be inferior to 65536.
  \param   num_reps   Number of 1D transfers that comprise the 2D transfer.

  \return         The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int pulp_idma_L2ToL1_2d(unsigned int src, unsigned int dst, unsigned short size, unsigned int src_stride, unsigned int dst_stride, unsigned int num_reps);
static inline int pulp_cl_idma_L2ToL1_2d(unsigned int src, unsigned int dst, unsigned short size, unsigned int src_stride, unsigned int dst_stride, unsigned int num_reps);

/** DMA-based zeromem using the "init" protocol.
 *
  \param   dst      Address in memory to fill with zeros. There is no restriction on memory alignment.
  \param   size     Number of bytes to be transfered. The only restriction is that this size must fit 16 bits, i.e. must be inferior to 65536.
  \param   dst_prot protocol with which the destination memory is attached (should be AXI or OBI)

  \return           The identifier of the transfer. This can be used with plp_dma_wait to wait for the completion of this transfer.
  */
static inline int pulp_idma_zeromem(unsigned int dst, unsigned short size, idma_prot_t dst_prot);
static inline int pulp_cl_idma_zeromem(unsigned int dst, unsigned short size, idma_prot_t dst_prot);
/** @name DMA wait functions
 */

/** DMA barrier.
 * This blocks the core until no transfer is on-going in the DMA.
 */
static inline void plp_dma_barrier();
static inline void plp_cl_dma_barrier();

/** DMA wait.
  * This blocks the core until the specified transfer is finished.
  *
  \param   counter  The counter ID identifying the transfer. This has been returned from an enqueued transfer (e.g. plp_dma_l2ToL1_2d)
 */
static inline void plp_dma_wait(unsigned int dma_tx_id);
static inline void plp_cl_dma_wait(unsigned int dma_tx_id);
//!@}


/** @name iDMA low-level functions.
  * This can be used instead of the high-level ones in order to have more control over the DMA features.
 */

/**
 * iDMA configuration generation
 * A standard memcpy will set all of these values - except for src and dest protocol - to 0.
 *
  \param  decouple   if set to true, there is no longer exactly one AXI write_request issued for
                     every read request. This mode can improve performance of unaligned transfers when crossing
                     the AXI page boundaries.
  \param  deburst    if set, the DMA will split all bursts in single transfers
  \param  serialize  if set, the DMA will only send AX belonging to a given Arbitrary 1D burst request
                     at a time. This is default behavior to prevent deadlocks. Setting `serialize` to
                     zero violates the AXI4+ATOP specification.
  \param  num_dim    number of dimensions: 1, 2 or 3. Invalid values will be treated as 1.
  \param  src_prot   Source protocol: AXI for transfer from L2, OBI for transfer from L1, INIT for zeromem
  \param  dst_prot   Destination protocol: AXI for transfer to L2, OBI for transfer to L1, INIT for transfer to /dev/null
  \return            The generated configuration
  */
static inline unsigned int pulp_idma_get_conf(unsigned int decouple_aw, unsigned int decouple_rw, unsigned int n_d, idma_prot_t src_prot, idma_prot_t dst_prot);


/**
 * Setting only source and destination protocols for a given transfer configuration
 *
  \param  conf       the configuration on which to set the source and destination protocols
  \param  src_prot   Source protocol: AXI for transfer from L2, OBI for transfer from L1, INIT for zeromem
  \param  dst_prot   Destination protocol: AXI for transfer to L2, OBI for transfer to L1, INIT for transfer to /dev/null
  \return            The generated configuration
  */
static inline unsigned int pulp_idma_set_conf_prot(unsigned int conf, idma_prot_t src_prot, idma_prot_t dst_prot);

/**
 * iDMA transfer status
 *
  \param  dma_tx_id  The dma transfer identifier
  \return            transfer status. 1 if complete, 0 if still ongoing or waiting.
  */
static inline unsigned int pulp_idma_tx_cplt(unsigned int dma_tx_id);
static inline unsigned int pulp_cl_idma_tx_cplt(unsigned int dma_tx_id);


/**
 * iDMA 2D memory transfer
 * Launches a standard 2D memory transfer
 *
  \param  dst_addr   The destination address
  \param  src_addr   The source address
  \param  num_bytes  The number bytes (per stride)
  \param  dst_stride The stride at the destination
  \param  src_stride The stride at the source
  \param  num_reps   The number of repetitions
  \return            The dma transfer identifier
  */
//static inline unsigned int pulp_idma_memcpy_2d(unsigned int const dst_addr, unsigned int const src_addr, unsigned int num_bytes, unsigned int dst_stride, unsigned int src_stride, unsigned int num_reps);


/**
 * iDMA advanced memory transfer
 * Launches a 1D memory transfer with special configuration options
 *
  \param  dst_addr   The destination address
  \param  src_addr   The source address
  \param  num_bytes  The number bytes
  \param  decouple   if set to true, there is no longer exactly one AXI write_request issued for
                     every read request. This mode can improve performance of unaligned transfers when crossing
                     the AXI page boundaries.
  \param  deburst    if set, the DMA will split all bursts in single transfers
  \param  serialize  if set, the DMA will only send AX belonging to a given Arbitrary 1D burst request
                     at a time. This is default behavior to prevent deadlocks. Setting `serialize` to
                     zero violates the AXI4+ATOP specification.
  \param  twod       if set, the DMA will execute a 2D transfer
  \param  dst_stride if 2D, the stride at the destination
  \param  src_stride if 2D, the stride at the source
  \param  num_reps   if 2D, the number of repetitions
  \return            The dma trasfer identifier
  */
//static inline unsigned int pulp_idma_memcpy_advanced(unsigned int const dst_addr, unsigned int const src_addr, unsigned int num_bytes, unsigned int decouple, unsigned int deburst, unsigned int serialize, unsigned int twod, unsigned int dst_stride, unsigned int src_stride, unsigned int num_reps);

/** Return the DMA status.
 *
  \return             DMA status. 1 means there are still on-going transfers, 0 means nothing is on-going.
  */
static inline unsigned int plp_dma_status();
static inline unsigned int plp_cl_dma_status();


//!@}


/// @cond IMPLEM

#if ARCHI_HAS_DMA_DEMUX
#define DMA_DEMUX_ADDR ARCHI_IDMA_DEMUX_ADDR
#endif
#define DMA_ADDR ARCHI_IDMA_EXT_ADDR

#if defined(__riscv__) && !defined(RV_ISA_RV32) && !defined(__LLVM__)
#define DMA_WRITE(value, offset) __builtin_pulp_OffsetedWrite((value), (int *)DMA_ADDR, (offset))
#define DMA_READ(offset) __builtin_pulp_OffsetedRead((int *)DMA_ADDR, (offset))
#ifdef ARCHI_HAS_DMA_DEMUX
#define DMA_CL_WRITE(value, offset) __builtin_pulp_OffsetedWrite((value), (int *)DMA_DEMUX_ADDR, (offset))
#define DMA_CL_READ(offset) __builtin_pulp_OffsetedRead((int *)DMA_DEMUX_ADDR, (offset))
#endif
#else
#define DMA_WRITE(value, offset) pulp_write32(DMA_ADDR + (offset), (value))
#define DMA_READ(offset) pulp_read32(DMA_ADDR + (offset))
#ifdef ARCHI_HAS_DMA_DEMUX
#define DMA_CL_WRITE(value, offset) pulp_write32(DMA_DEMUX_ADDR + (offset), (value))
#define DMA_CL_READ(offset) pulp_read32(DMA_DEMUX_ADDR + (offset))
#endif
#endif
// if we don't have the peripheral demux, the cluster write/read functions are equal to the regular versions
#ifndef ARCHI_HAS_DMA_DEMUX
#define DMA_CL_WRITE(value, offset) DMA_WRITE(value, offset)
#define DMA_CL_READ(offset) DMA_READ(value, offset)
#endif

static inline int plp_dma_memcpy(dma_ext_t ext, unsigned int loc, unsigned short size, int ext2loc) {
  if (ext2loc)
    return pulp_idma_L2ToL1(ext, loc, size);
  else
    return pulp_idma_L1ToL2(loc, ext, size);
}
static inline int plp_cl_dma_memcpy(dma_ext_t ext, unsigned int loc, unsigned short size, int ext2loc) {
  if (ext2loc)
    return pulp_cl_idma_L2ToL1(ext, loc, size);
  else
    return pulp_cl_idma_L1ToL2(loc, ext, size);
}

static inline int plp_dma_l1ToExt(dma_ext_t ext, unsigned int loc, unsigned short size) {
  return pulp_idma_L1ToL2(loc, ext, size);
}
static inline int plp_cl_dma_l1ToExt(dma_ext_t ext, unsigned int loc, unsigned short size) {
  return pulp_cl_idma_L1ToL2(loc, ext, size);
}

static inline int plp_dma_extToL1(unsigned int loc, dma_ext_t ext, unsigned short size) {
  return pulp_idma_L2ToL1(ext, loc, size);
}
static inline int plp_cl_dma_extToL1(unsigned int loc, dma_ext_t ext, unsigned short size) {
  return pulp_cl_idma_L2ToL1(ext, loc, size);
}

static inline int plp_dma_memcpy_2d(dma_ext_t ext, unsigned int loc, unsigned int size, unsigned int stride, unsigned int length, int ext2loc) {
  if (ext2loc)
    return pulp_idma_L2ToL1_2d(ext, loc, length, stride, length, size/length);
  else
    return pulp_idma_L1ToL2_2d(loc, ext, length, length, stride, size/length);
}
static inline int plp_cl_dma_memcpy_2d(dma_ext_t ext, unsigned int loc, unsigned int size, unsigned int stride, unsigned int length, int ext2loc) {
  if (ext2loc)
    return pulp_cl_idma_L2ToL1_2d(ext, loc, length, stride, length, size/length);
  else
    return pulp_cl_idma_L1ToL2_2d(loc, ext, length, length, stride, size/length);
}

static inline int plp_dma_l1ToExt_2d(dma_ext_t ext, unsigned int loc, unsigned short size, unsigned short stride, unsigned short length) {
  return pulp_idma_L1ToL2_2d(loc, ext, length, length, stride, size/length);
}
static inline int plp_cl_dma_l1ToExt_2d(dma_ext_t ext, unsigned int loc, unsigned short size, unsigned short stride, unsigned short length) {
  return pulp_cl_idma_L1ToL2_2d(loc, ext, length, length, stride, size/length);
}

static inline int plp_dma_extToL1_2d(unsigned int loc, dma_ext_t ext, unsigned short size, unsigned short stride, unsigned short length) {
  return pulp_idma_L2ToL1_2d(loc, ext, length, stride, length, size/length);
}
static inline int plp_cl_dma_extToL1_2d(unsigned int loc, dma_ext_t ext, unsigned short size, unsigned short stride, unsigned short length) {
  return pulp_cl_idma_L2ToL1_2d(loc, ext, length, stride, length, size/length);
}

static inline unsigned int pulp_idma_set_conf_prot(unsigned int conf, idma_prot_t src_prot, idma_prot_t dst_prot){
#if defined(__riscv__)
  conf = __builtin_bitinsert(conf, src_prot,     3, IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET);
  conf = __builtin_bitinsert(conf, dst_prot,     3, IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET);
#else
  conf &= (~((IDMA_REG32_3D_CONF_SRC_PROTOCOL_MASK | (IDMA_REG32_3D_CONF_DST_PROTOCOL_MASK << (IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET - IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET))) << IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET)); // set the relevant bits to 0
  conf |= ((src_prot << IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET) | (dst_prot << IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET));
#endif
  return conf;
}

static inline unsigned int pulp_idma_get_conf(unsigned int decouple_aw, unsigned int decouple_rw, unsigned int n_d, idma_prot_t src_prot, idma_prot_t dst_prot){
  unsigned int conf;
#if defined(__riscv__)
  conf = __builtin_bitinsert(0,    decouple_aw,  1, IDMA_REG32_3D_CONF_DECOUPLE_AW_BIT);
  conf = __builtin_bitinsert(conf, decouple_rw,  1, IDMA_REG32_3D_CONF_DECOUPLE_RW_BIT);
  conf = __builtin_bitinsert(conf, n_d,          2, IDMA_REG32_3D_CONF_ENABLE_ND_OFFSET);
  conf = __builtin_bitinsert(conf, src_prot,     3, IDMA_REG32_3D_CONF_SRC_PROTOCOL_OFFSET);
  conf = __builtin_bitinsert(conf, dst_prot,     3, IDMA_REG32_3D_CONF_DST_PROTOCOL_OFFSET);
  // TODO: add burst length reduction
#else
  conf = (((decouple_rw & 0x1)<<IDMA_REG32_3D_CONF_DECOUPLE_RW_BIT) | ((decouple_aw & 0x1)<<IDMA_REG32_3D_CONF_DECOUPLE_AW_BIT) | ((n_d & 0x3)<<IDMA_REG32_3D_CONF_ND_OFFSET) | ((src_prot & 0x7)<<IDMA_3D_CONF_SRC_PROTOCOL_OFFSET) | ((dst_prot & 0x7)<<IDMA_3D_CONF_DST_PROTOCOL_OFFSET));
#endif
  return conf;
}

static inline unsigned int pulp_idma_tx_cplt(unsigned int dma_tx_id) {
  unsigned int done_id = DMA_READ(IDMA_REG32_3D_DONE_ID_0_REG_OFFSET);
  unsigned int my_id = dma_tx_id & IDMA_ID_MASK;
  if (done_id >> (IDMA_ID_COUNTER_WIDTH-1) == my_id >> (IDMA_ID_COUNTER_WIDTH-1)) {
    return my_id <= done_id;
  } else {
    return ((done_id & (IDMA_ID_MASK - (1<<(IDMA_ID_COUNTER_WIDTH-1))) < (1<<(IDMA_ID_COUNTER_WIDTH-2))));
  }
}
static inline unsigned int pulp_cl_idma_tx_cplt(unsigned int dma_tx_id) {
  unsigned int done_id = DMA_CL_READ(IDMA_REG32_3D_DONE_ID_0_REG_OFFSET);
  unsigned int my_id = dma_tx_id & IDMA_ID_MASK;
  if (done_id >> (IDMA_ID_COUNTER_WIDTH-1) == my_id >> (IDMA_ID_COUNTER_WIDTH-1)) {
    return my_id <= done_id;
  } else {
    return ((done_id & (IDMA_ID_MASK - (1<<(IDMA_ID_COUNTER_WIDTH-1))) < (1<<(IDMA_ID_COUNTER_WIDTH-2))));
  }
}


static inline unsigned int plp_dma_status() {
  return DMA_READ(IDMA_REG32_3D_STATUS_0_REG_OFFSET);
}
static inline unsigned int plp_cl_dma_status() {
  return DMA_CL_READ(IDMA_REG32_3D_STATUS_0_REG_OFFSET);
}

static inline void plp_dma_wait(unsigned int dma_tx_id) {
  while(!pulp_idma_tx_cplt(dma_tx_id)) {
    eu_evt_maskWaitAndClr(1 << IDMA_EVENT);
  }
  return;
}
static inline void plp_cl_dma_wait(unsigned int dma_tx_id) {
  while(!pulp_cl_idma_tx_cplt(dma_tx_id)) {
    eu_evt_maskWaitAndClr(1 << IDMA_EVENT);
  }
  return;
}

static inline int pulp_idma_memcpy(unsigned int src, unsigned int dst, unsigned int size, idma_prot_t src_prot, idma_prot_t dst_prot) {
  unsigned int dma_tx_id;
  unsigned int cfg = pulp_idma_set_conf_prot(IDMA_DEFAULT_CONFIG, src_prot, dst_prot);
  DMA_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);

  asm volatile("" : : : "memory");
  // Launch TX
  if (dst_prot == IDMA_PROT_AXI)
    dma_tx_id = DMA_READ(IDMA_REG32_3D_NEXT_ID_0_REG_OFFSET);
  else
    dma_tx_id = DMA_READ(IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET);
  return dma_tx_id;
}
static inline int pulp_cl_idma_memcpy(unsigned int src, unsigned int dst, unsigned int size, idma_prot_t src_prot, idma_prot_t dst_prot) {
  unsigned int dma_tx_id;
  unsigned int cfg = pulp_idma_set_conf_prot(IDMA_DEFAULT_CONFIG, src_prot, dst_prot);
  DMA_CL_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_CL_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);

  asm volatile("" : : : "memory");
  // Launch TX
  if (dst_prot == IDMA_PROT_AXI)
    dma_tx_id = DMA_CL_READ(IDMA_REG32_3D_NEXT_ID_0_REG_OFFSET);
  else
    dma_tx_id = DMA_CL_READ(IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET);
  return dma_tx_id;
}

static inline int pulp_idma_L1ToL2(unsigned int src, unsigned int dst, unsigned short size) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG_L1TOL2;
  DMA_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);

  asm volatile("" : : : "memory");
  // Launch TX
  dma_tx_id = DMA_READ(IDMA_REG32_3D_NEXT_ID_0_REG_OFFSET);

  return dma_tx_id;
}
static inline int pulp_cl_idma_L1ToL2(unsigned int src, unsigned int dst, unsigned short size) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG_L1TOL2;
  DMA_CL_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_CL_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);

  asm volatile("" : : : "memory");
  // Launch TX
  dma_tx_id = DMA_CL_READ(IDMA_REG32_3D_NEXT_ID_0_REG_OFFSET);

  return dma_tx_id;
}

static inline int pulp_idma_L2ToL1(unsigned int src, unsigned int dst, unsigned short size) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG_L2TOL1;
  DMA_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);

  asm volatile("" : : : "memory");
  // Launch TX
  dma_tx_id = DMA_READ(IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET);

  return dma_tx_id;
}
static inline int pulp_cl_idma_L2ToL1(unsigned int src, unsigned int dst, unsigned short size) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG_L2TOL1;
  DMA_CL_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_CL_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);

  asm volatile("" : : : "memory");
  // Launch TX
  dma_tx_id = DMA_CL_READ(IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET);

  return dma_tx_id;
}

static inline int pulp_idma_L1ToL1(unsigned int src, unsigned int dst, unsigned short size) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG_L1TOL1;
  DMA_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);

  asm volatile("" : : : "memory");
  // Launch TX
  dma_tx_id = DMA_READ(IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET);

  return dma_tx_id;
}
static inline int pulp_cl_idma_L1ToL1(unsigned int src, unsigned int dst, unsigned short size) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG_L1TOL1;
  DMA_CL_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_CL_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);

  asm volatile("" : : : "memory");
  // Launch TX
  dma_tx_id = DMA_CL_READ(IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET);

  return dma_tx_id;
}

static inline int pulp_idma_memcpy_2d(unsigned int src, unsigned int dst, unsigned int size, unsigned int src_stride, unsigned int dst_stride, unsigned int num_reps, idma_prot_t src_prot, idma_prot_t dst_prot) {
    unsigned int dma_tx_id;
    unsigned int cfg = IDMA_DEFAULT_CONFIG_2D;
    cfg = pulp_idma_set_conf_prot(cfg, src_prot, dst_prot);
    DMA_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
    DMA_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
    DMA_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
    DMA_WRITE(src_stride, IDMA_REG32_3D_SRC_STRIDE_2_LOW_REG_OFFSET);
    DMA_WRITE(dst_stride, IDMA_REG32_3D_DST_STRIDE_2_LOW_REG_OFFSET);
    DMA_WRITE(num_reps, IDMA_REG32_3D_REPS_2_LOW_REG_OFFSET);

    asm volatile("" : : : "memory");
    // Launch TX
    if (dst_prot == IDMA_PROT_AXI)
      dma_tx_id = DMA_READ(IDMA_REG32_3D_NEXT_ID_0_REG_OFFSET);
    else
      dma_tx_id = DMA_READ(IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET);
    return dma_tx_id;
}
static inline int pulp_cl_idma_memcpy_2d(unsigned int src, unsigned int dst, unsigned int size, unsigned int src_stride, unsigned int dst_stride, unsigned int num_reps, idma_prot_t src_prot, idma_prot_t dst_prot) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG_2D;
  cfg = pulp_idma_set_conf_prot(cfg, src_prot, dst_prot);
  DMA_CL_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_CL_WRITE(src_stride, IDMA_REG32_3D_SRC_STRIDE_2_LOW_REG_OFFSET);
  DMA_CL_WRITE(dst_stride, IDMA_REG32_3D_DST_STRIDE_2_LOW_REG_OFFSET);
  DMA_CL_WRITE(num_reps, IDMA_REG32_3D_REPS_2_LOW_REG_OFFSET);

  asm volatile("" : : : "memory");
  // Launch TX
  if (dst_prot == IDMA_PROT_AXI)
    dma_tx_id = DMA_CL_READ(IDMA_REG32_3D_NEXT_ID_0_REG_OFFSET);
  else
    dma_tx_id = DMA_CL_READ(IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET);
  return dma_tx_id;
}

static inline int pulp_idma_L1ToL2_2d(unsigned int src, unsigned int dst, unsigned short size, unsigned int src_stride, unsigned int dst_stride, unsigned int num_reps) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG_L1TOL2_2D;
  DMA_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);
  DMA_WRITE(src_stride, IDMA_REG32_3D_SRC_STRIDE_2_LOW_REG_OFFSET);
  DMA_WRITE(dst_stride, IDMA_REG32_3D_DST_STRIDE_2_LOW_REG_OFFSET);
  DMA_WRITE(num_reps, IDMA_REG32_3D_REPS_2_LOW_REG_OFFSET);

  asm volatile("" : : : "memory");
  dma_tx_id = DMA_READ(IDMA_REG32_3D_NEXT_ID_0_REG_OFFSET);
  return dma_tx_id;
}
static inline int pulp_cl_idma_L1ToL2_2d(unsigned int src, unsigned int dst, unsigned short size, unsigned int src_stride, unsigned int dst_stride, unsigned int num_reps) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG_L1TOL2_2D;
  DMA_CL_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_CL_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);
  DMA_CL_WRITE(src_stride, IDMA_REG32_3D_SRC_STRIDE_2_LOW_REG_OFFSET);
  DMA_CL_WRITE(dst_stride, IDMA_REG32_3D_DST_STRIDE_2_LOW_REG_OFFSET);
  DMA_CL_WRITE(num_reps, IDMA_REG32_3D_REPS_2_LOW_REG_OFFSET);

  asm volatile("" : : : "memory");
  dma_tx_id = DMA_CL_READ(IDMA_REG32_3D_NEXT_ID_0_REG_OFFSET);
  return dma_tx_id;
}

static inline int pulp_idma_L2ToL1_2d(unsigned int src, unsigned int dst, unsigned short size, unsigned int src_stride, unsigned int dst_stride, unsigned int num_reps) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG_L2TOL1_2D;
  DMA_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);
  DMA_WRITE(src_stride, IDMA_REG32_3D_SRC_STRIDE_2_LOW_REG_OFFSET);
  DMA_WRITE(dst_stride, IDMA_REG32_3D_DST_STRIDE_2_LOW_REG_OFFSET);
  DMA_WRITE(num_reps, IDMA_REG32_3D_REPS_2_LOW_REG_OFFSET);

  asm volatile("" : : : "memory");
  dma_tx_id = DMA_READ(IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET);
  return dma_tx_id;
}
static inline int pulp_cl_idma_L2ToL1_2d(unsigned int src, unsigned int dst, unsigned short size, unsigned int src_stride, unsigned int dst_stride, unsigned int num_reps) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG_L2TOL1_2D;
  DMA_CL_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_CL_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);
  DMA_CL_WRITE(src_stride, IDMA_REG32_3D_SRC_STRIDE_2_LOW_REG_OFFSET);
  DMA_CL_WRITE(dst_stride, IDMA_REG32_3D_DST_STRIDE_2_LOW_REG_OFFSET);
  DMA_CL_WRITE(num_reps, IDMA_REG32_3D_REPS_2_LOW_REG_OFFSET);

  asm volatile("" : : : "memory");
  dma_tx_id = DMA_CL_READ(IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET);
  return dma_tx_id;
}


static inline int pulp_idma_L1ToL1_2d(unsigned int src, unsigned int dst, unsigned short size, unsigned int src_stride, unsigned int dst_stride, unsigned int num_reps) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG_L1TOL1_2D;
  DMA_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);
  DMA_WRITE(src_stride, IDMA_REG32_3D_SRC_STRIDE_2_LOW_REG_OFFSET);
  DMA_WRITE(dst_stride, IDMA_REG32_3D_DST_STRIDE_2_LOW_REG_OFFSET);
  DMA_WRITE(num_reps, IDMA_REG32_3D_REPS_2_LOW_REG_OFFSET);

  asm volatile("" : : : "memory");
  dma_tx_id = DMA_READ(IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET);
  return dma_tx_id;
}
static inline int pulp_cl_idma_L1ToL1_2d(unsigned int src, unsigned int dst, unsigned short size, unsigned int src_stride, unsigned int dst_stride, unsigned int num_reps) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG_L1TOL1_2D;
  DMA_CL_WRITE(src, IDMA_REG32_3D_SRC_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_CL_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);
  DMA_CL_WRITE(src_stride, IDMA_REG32_3D_SRC_STRIDE_2_LOW_REG_OFFSET);
  DMA_CL_WRITE(dst_stride, IDMA_REG32_3D_DST_STRIDE_2_LOW_REG_OFFSET);
  DMA_CL_WRITE(num_reps, IDMA_REG32_3D_REPS_2_LOW_REG_OFFSET);

  asm volatile("" : : : "memory");
  dma_tx_id = DMA_CL_READ(IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET);
  return dma_tx_id;
}

static inline int pulp_idma_zeromem(unsigned int dst, unsigned short size, idma_prot_t dst_prot) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG;
  cfg = pulp_idma_set_conf_prot(cfg, IDMA_PROT_INIT, dst_prot);
  DMA_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);
  if (dst_prot == IDMA_PROT_AXI)
    dma_tx_id = DMA_READ(IDMA_REG32_3D_NEXT_ID_0_REG_OFFSET);
  else
    dma_tx_id = DMA_READ(IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET);
  asm volatile("" : : : "memory");
  return dma_tx_id;
}
static inline int pulp_cl_idma_zeromem(unsigned int dst, unsigned short size, idma_prot_t dst_prot) {
  unsigned int dma_tx_id;
  unsigned int cfg = IDMA_DEFAULT_CONFIG;
  cfg = pulp_idma_set_conf_prot(cfg, IDMA_PROT_INIT, dst_prot);
  DMA_CL_WRITE(dst, IDMA_REG32_3D_DST_ADDR_LOW_REG_OFFSET);
  DMA_CL_WRITE(size, IDMA_REG32_3D_LENGTH_LOW_REG_OFFSET);
  DMA_CL_WRITE(cfg, IDMA_REG32_3D_CONF_REG_OFFSET);
  if (dst_prot == IDMA_PROT_AXI)
    dma_tx_id = DMA_CL_READ(IDMA_REG32_3D_NEXT_ID_0_REG_OFFSET);
  else
    dma_tx_id = DMA_CL_READ(IDMA_REG32_3D_NEXT_ID_1_REG_OFFSET);
  asm volatile("" : : : "memory");
  return dma_tx_id;
}


static inline void plp_dma_barrier() {
  while(plp_dma_status()) {
    eu_evt_maskWaitAndClr(1 << IDMA_EVENT);
  }
}
static inline void plp_cl_dma_barrier() {
  while(plp_cl_dma_status()) {
    eu_evt_maskWaitAndClr(1 << IDMA_EVENT);
  }
}

#endif // __HAL_IDMA_V2_H__