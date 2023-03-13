#
# Copyright (C) 2019 ETH Zurich, University of Bologna
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#include "archi/pulp.h"
#include "pulp.h"

    .section .text
    .global pos_init_entry
pos_init_entry:

    # Cluster PEs will also starts here to avoid aligning another entry point
    # Just re-route them to the right entry
#if defined(ARCHI_HAS_CLUSTER)
    csrr    a0, 0xF14
    andi    a1, a0, 0x1f
    srli    a0, a0, 5
#ifdef ARCHI_NO_FC
    // Writing registers for cluster boot
    li  x2, 0x10200000
    li  x3, 0x1
    la  x4, _start
    sw  x4, 0x40(x2)
    sw  x3, 8(x2)
#else
#ifdef ARCHI_CL_BOOT

      li      a2, ARCHI_FC_CID
      j       do_cl_boot
//      beq     a0, a2, do_cl_boot // FC will go there
//      bnez    a1, pe_start // cluster will go here

#else

#ifdef ARCHI_FC_CID
    li      a2, ARCHI_FC_CID
    bne     a0, a2, pe_start
#else
    bnez    a1, pe_start
#endif // ARCHI_FC_ID
#endif // ARCHI_CL_BOOT
#endif // ARCHI_NO_FC

#endif



    # Clear the bss segment
    la      t0, _bss_start
    la      t1, _bss_end
1:
    sw      zero,0(t0)
    addi    t0, t0, 4
    bltu    t0, t1, 1b



    # Stack initialization
    la   x2, stack



    /* Do all other initializations from C code */
    jal  x1, pos_init_start



.section .text

    # On all other chips we simply pass 0.
    addi  a0, x0, 0
    addi  a1, x0, 0

    # Jump to main program entry point (argc = a0, argv = a1).
    la    t2, main
    jalr  x1, t2
    mv    s0, a0

    /* Do all other deinitializations from C code */
    jal  x1, pos_init_stop

    # If program returns from main, call exit routine
    mv   a0, s0
    jal  x1, exit




.section .vectors, "ax"
.option norvc;


#ifdef ARCHI_CORE_HAS_1_10
    j __rt_handle_illegal_instr
#else
    j pos_no_irq_handler
#endif

    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler
    j pos_no_irq_handler


    .org 0x80
    .global _start
_start:
    jal x0, pos_init_entry


pos_illegal_instr:
    j __rt_handle_illegal_instr

pos_no_irq_handler:
    mret


    .global pos_semihosting_call
pos_semihosting_call:
    ebreak
    jr          ra



#if defined(ARCHI_HAS_CLUSTER)
pe_start:
    la   x2, cluster_stacks
    lw   x2, 0(x2)
    li   x3, CLUSTER_STACK_SIZE
    addi a1, a1, 1
    mul  x1, x3, a1
    add  x2, x2, x1
    j    cluster_entry_stub // fc will go here
#endif

do_cl_boot:
    li  x2, 0x10200000
    li  x3, 0x1
    la  x4, _start
    sw  x4, 0x40(x2)
    sw  x3, 8(x2)
    li x2, 0x1a109800
    sw x0, 0(x2)

loop:
    li x2, 0x1a109800
    sw x0, 0(x2)
    wfi
    j loop
