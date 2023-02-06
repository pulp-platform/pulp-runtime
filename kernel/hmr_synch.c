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

#include <pulp.h>

#define QUAUX(X) #X
#define QU(X) QUAUX(X)

#define HMR_STATE_ALLOC_SIZE 0xA0

void __attribute__((naked)) pos_hmr_store_state_to_stack() {

  __asm__ __volatile__ (
    // Allocate space on the stack
    "add  sp, sp, -" QU(HMR_STATE_ALLOC_SIZE) " \n\t"
    
    // Store registers to stack
    // zero not stored as hardwired          //  x0
    "sw   ra,  0x00(sp) \n\t"                //  x1
    // sp stored to HMR once complete        //  x2
    "sw   gp,  0x04(sp) \n\t"                //  x3
    "sw   tp,  0x08(sp) \n\t"                //  x4
    "sw   t0,  0x0C(sp) \n\t"                //  x5
    "sw   t1,  0x10(sp) \n\t"                //  x6
    "sw   t2,  0x14(sp) \n\t"                //  x7
    "sw   x8,  0x18(sp) \n\t"                //  fp
    "sw   s1,  0x1C(sp) \n\t"                //  x9
    "sw   a0,  0x20(sp) \n\t"                // x10
    "sw   a1,  0x24(sp) \n\t"                // x11
    "sw   a2,  0x28(sp) \n\t"                // x12
    "sw   a3,  0x2C(sp) \n\t"                // x13
    "sw   a4,  0x30(sp) \n\t"                // x14
    "sw   a5,  0x34(sp) \n\t"                // x15
    "sw   a6,  0x38(sp) \n\t"                // x16
    "sw   a7,  0x3C(sp) \n\t"                // x17
    "sw   s2,  0x40(sp) \n\t"                // x18
    "sw   s3,  0x44(sp) \n\t"                // x19
    "sw   s4,  0x48(sp) \n\t"                // x20
    "sw   s5,  0x4C(sp) \n\t"                // x21
    "sw   s6,  0x50(sp) \n\t"                // x22
    "sw   s7,  0x54(sp) \n\t"                // x23
    "sw   s8,  0x58(sp) \n\t"                // x24
    "sw   s9,  0x5C(sp) \n\t"                // x25
    "sw   s10, 0x60(sp) \n\t"                // x26
    "sw   s11, 0x64(sp) \n\t"                // x27
    "sw   t3,  0x68(sp) \n\t"                // x28
    "sw   t4,  0x6C(sp) \n\t"                // x29
    "sw   t5,  0x70(sp) \n\t"                // x30
    "sw   t6,  0x74(sp) \n\t"                // x31

    // Manually store necessary CSRs
    "csrr t1,  0x341 \n\t"        // mepc
    "csrr t2,  0x300 \n\t"        // mstatus
    "sw   t1,  0x78(sp) \n\t"                // mepc
    "csrr t1,  0x304 \n\t"        // mie
    "sw   t2,  0x7C(sp) \n\t"                // mstatus
    "csrr t2,  0x305 \n\t"        // mtvec
    "sw   t1,  0x80(sp) \n\t"                // mie
    "csrr t1,  0x340 \n\t"        // mscratch
    "sw   t2,  0x84(sp) \n\t"                // mtvec
    "csrr t2,  0x342 \n\t"        // mcause
    "sw   t1,  0x88(sp) \n\t"                // mscratch
    "csrr t1,  0x343 \n\t"        // mtval
    "sw   t2,  0x8C(sp) \n\t"                // mcause
#ifdef __ibex__
    "csrr t2,  0x7d0 \n\t"        // miex
#endif // __ibex__
    "sw   t1,  0x90(sp) \n\t"                // mtval
#ifdef __ibex__
    "csrr t1,  0x7d1 \n\t"        // mtvecx
    "sw   t2,  0x94(sp) \n\t"                // miex
    "sw   t1,  0x98(sp) \n\t"                // mtvecx
#endif // __ibex__

    : : : "memory");
}


// loads state from stack, except for ra which is stored at `0x00(sp)` before and `-HMR_STATE_ALLOC_SIZE(sp)` afterwards
void __attribute__((naked)) pos_hmr_load_state_from_stack() {
  __asm__ __volatile__ (
    // Manually load necessary CSRs
    "lw   t1,  0x78(sp) \n\t"                // mepc
    "lw   t2,  0x7C(sp) \n\t"                // mstatus
    "csrw 0x341,  t1 \n\t"        // mepc
    "lw   t1,  0x80(sp) \n\t"                // mie
    "csrw 0x300,  t2 \n\t"        // mstatus
    "lw   t2,  0x84(sp) \n\t"                // mtvec
    "csrw 0x304,  t1 \n\t"        // mie
    "lw   t1,  0x88(sp) \n\t"                // mscratch
    "csrw 0x305,  t2 \n\t"        // mtvec
    "lw   t2,  0x8C(sp) \n\t"                // mcause
    "csrw 0x340,  t1 \n\t"        // mscratch
    "lw   t1,  0x90(sp) \n\t"                // mtval
    "csrw 0x342,  t2 \n\t"        // mcause
#ifdef __ibex__
    "lw   t2,  0x94(sp) \n\t"                // miex
#endif // __ibex__
    "csrw 0x343,  t1 \n\t"        // mtval
#ifdef __ibex__
    "lw   t1,  0x98(sp) \n\t"                // mtvecx
    "csrw 0x7d0,  t2 \n\t"        // miex
    "csrw 0x7d1,  t1 \n\t"        // mtvecx
#endif // __ibex__

    // Load registers from stack
    // zero not loaded as hardwired          //  x0
    // ra not touched
    // "lw   ra,  0x00(sp) \n\t"                //  x1
    // sp loaded from HMR regs above         //  x2
    "lw   gp,  0x04(sp) \n\t"                //  x3
    "lw   tp,  0x08(sp) \n\t"                //  x4
    "lw   t0,  0x0C(sp) \n\t"                //  x5
    "lw   t1,  0x10(sp) \n\t"                //  x6
    "lw   t2,  0x14(sp) \n\t"                //  x7
    "lw   x8,  0x18(sp) \n\t"                //  fp
    "lw   s1,  0x1C(sp) \n\t"                //  x9
    "lw   a0,  0x20(sp) \n\t"                // x10
    "lw   a1,  0x24(sp) \n\t"                // x11
    "lw   a2,  0x28(sp) \n\t"                // x12
    "lw   a3,  0x2C(sp) \n\t"                // x13
    "lw   a4,  0x30(sp) \n\t"                // x14
    "lw   a5,  0x34(sp) \n\t"                // x15
    "lw   a6,  0x38(sp) \n\t"                // x16
    "lw   a7,  0x3C(sp) \n\t"                // x17
    "lw   s2,  0x40(sp) \n\t"                // x18
    "lw   s3,  0x44(sp) \n\t"                // x19
    "lw   s4,  0x48(sp) \n\t"                // x20
    "lw   s5,  0x4C(sp) \n\t"                // x21
    "lw   s6,  0x50(sp) \n\t"                // x22
    "lw   s7,  0x54(sp) \n\t"                // x23
    "lw   s8,  0x58(sp) \n\t"                // x24
    "lw   s9,  0x5C(sp) \n\t"                // x25
    "lw   s10, 0x60(sp) \n\t"                // x26
    "lw   s11, 0x64(sp) \n\t"                // x27
    "lw   t3,  0x68(sp) \n\t"                // x28
    "lw   t4,  0x6C(sp) \n\t"                // x29
    "lw   t5,  0x70(sp) \n\t"                // x30
    "lw   t6,  0x74(sp) \n\t"                // x31

    // Release space on the stack
    "add  sp, sp, " QU(HMR_STATE_ALLOC_SIZE) " \n\t"
    : : : "memory");
}

void __attribute__((interrupt)) pos_hmr_tmr_reload() {
  // get sp from tmr reg
  __asm__ __volatile__(
    "csrr t0, 0xf14 \n\t" // Read core id
    "li t1, " QU(ARCHI_HMR_ADDR + HMR_CORE_OFFSET) " \n\t"
    "andi t0, t0, 0x01f \n\t"
    "sll t0, t0, " QU(HMR_CORE_SLL) " \n\t"
    "add t0, t0, t1 \n\t"
    "lw sp, " QU(HMR_CORE_REGS_SP_STORE_REG_OFFSET) "(t0) \n\t"
    "mv ra, t0 \n\t"
  : : : "memory");

  pos_hmr_load_state_from_stack();
  
  // set tmr reg to 0
  __asm__ __volatile__(
    "sw zero, " QU(HMR_CORE_REGS_SP_STORE_REG_OFFSET) "(ra) \n\t"
    "lw ra, -" QU(HMR_STATE_ALLOC_SIZE) "(sp) \n\t"
  : : : "memory");

  // mret handled by __attribute((interrupt))  
  // __asm__ __volatile__("mret" : : : "memory");
}

void __attribute__((naked)) pos_hmr_tmr_irq() {
  pos_hmr_store_state_to_stack();
  
  // store sp to hmr core reg
  __asm__ __volatile__(
    "csrr t0, 0xf14 \n\t" // Read core id
    "li t1, " QU(ARCHI_HMR_ADDR + HMR_CORE_OFFSET) " \n\t"
    "andi t0, t0, 0x01f \n\t"
    "sll t0, t0, " QU(HMR_CORE_SLL) " \n\t"
    "add t0, t0, t1 \n\t"
    "sw sp, " QU(HMR_CORE_REGS_SP_STORE_REG_OFFSET) "(t0) \n\t"
  : : : "memory");
  
  // several nops to delay and allow for core reset
  __asm__ __volatile__(
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
  : : : "memory");
  pos_hmr_tmr_reload();
}

void __attribute__((naked)) pos_hmr_tmr_synch() {
  if (TMR_IS_MAIN_CORE(core_id())) {
    eu_bar_setup(eu_bar_addr(TMR_BARRIER_ID(TMR_GROUP_ID(core_id()))), TMR_BARRIER_SETUP(TMR_GROUP_ID(core_id())));
    // eu_bar_setup(eu_bar_addr(0), hmr_get_active_cores(0));
  }
  pos_hmr_store_state_to_stack();
  
  // store sp to hmr core reg
  __asm__ __volatile__(
    "csrr t0, 0xf14 \n\t" // Read core id
    "li t1, " QU(ARCHI_HMR_ADDR + HMR_CORE_OFFSET) " \n\t"
    "andi t0, t0, 0x01f \n\t"
    "sll t0, t0, " QU(HMR_CORE_SLL) " \n\t"
    "add t0, t0, t1 \n\t"
    "sw sp, " QU(HMR_CORE_REGS_SP_STORE_REG_OFFSET) "(t0) \n\t"
  : : : "memory");
  
  // enter barrier -> this should lock the cores together
  eu_bar_trig_wait_clr(eu_bar_addr(TMR_BARRIER_ID(TMR_GROUP_ID(core_id()))));

  // several nops to delay and allow for core reset
  __asm__ __volatile__(
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
  : : : "memory");

  pos_hmr_tmr_reload();
}

int hmr_tmr_critical_section(int (*function_handle)()) {
  int ret = 0; 
  if (TMR_IS_MAIN_CORE(core_id())) {
    // enter critical section
    hmr_enable_tmr(0, TMR_GROUP_ID(core_id()));

    // do critical stuff
    ret += function_handle();

    // exit critical section
    hmr_disable_tmr(0, TMR_GROUP_ID(core_id()));

  }
  synch_barrier();

  return ret;
}

// void pos_hmr_tmr_unsync() {

//   // Update event unit mask
//   // write unsync to hmr tmr ctrl reg
//   if (!TMR_IS_MAIN_CORE(core_id())) {
//     // get sp from a core reg
//     if (sp == 0) {
//       j 
//     }
//     pos_hmr_load_state_from_stack();
//   //     mret?
//   }
// }

// void pos_hmr_create_checkpoint() {
//   // get checkpoint addr (or alloc the space?) --> will be complex for stack...
//   // pos_hmr_store_state_to(addr)
//   // store addr to dmr reg? --> need to properly manage this...
// }

// void pos_hmr_load_checkpoint() {
//   // load addr from dmr reg?
//   // pos_hmr_load_state_from(addr)
//   // mret? ret?
// }
