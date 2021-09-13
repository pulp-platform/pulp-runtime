#
# Copyright (C) 2021 ETH Zurich, University of Bologna
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

  .global pos_TCLS_unload
pos_TCLS_unload:

# TODO: check if TCLS is supported via define
# TODO: check if TCLS is enabled currently

  add  sp, sp, -0xA0

  # zero not stored as hardwired    #  x0
  sw   ra,  0x00(sp)                #  x1
  # sp stored to TCLS once complete #  x2
  sw   gp,  0x04(sp)                #  x3
  sw   tp,  0x08(sp)                #  x4
  sw   t0,  0x0C(sp)                #  x5
  sw   t1,  0x10(sp)                #  x6
  sw   t2,  0x14(sp)                #  x7
  sw   x8,  0x18(sp)                #  fp
  sw   s1,  0x1C(sp)                #  x9
  sw   a0,  0x20(sp)                # x10
  sw   a1,  0x24(sp)                # x11
  sw   a2,  0x28(sp)                # x12
  sw   a3,  0x2C(sp)                # x13
  sw   a4,  0x30(sp)                # x14
  sw   a5,  0x34(sp)                # x15
  sw   a6,  0x38(sp)                # x16
  sw   a7,  0x3C(sp)                # x17
  sw   s2,  0x40(sp)                # x18
  sw   s3,  0x44(sp)                # x19
  sw   s4,  0x48(sp)                # x20
  sw   s5,  0x4C(sp)                # x21
  sw   s6,  0x50(sp)                # x22
  sw   s7,  0x54(sp)                # x23
  sw   s8,  0x58(sp)                # x24
  sw   s9,  0x5C(sp)                # x25
  sw   s10, 0x60(sp)                # x26
  sw   s11, 0x64(sp)                # x27
  sw   t3,  0x68(sp)                # x28
  sw   t4,  0x6C(sp)                # x29
  sw   t5,  0x70(sp)                # x30
  sw   t6,  0x74(sp)                # x31

  csrr t1,  0x341        # mepc
  csrr t2,  0x300        # mstatus
  sw   t1,  0x78(sp)                # mepc
  csrr t1,  0x304        # mie
  sw   t2,  0x7C(sp)                # mstatus
  csrr t2,  0x305        # mtvec
  sw   t1,  0x80(sp)                # mie
  csrr t1,  0x340        # mscratch
  sw   t2,  0x84(sp)                # mtvec
  csrr t2,  0x342        # mcause
  sw   t1,  0x88(sp)                # mscratch
  csrr t1,  0x343        # mtval
  sw   t2,  0x8C(sp)                # mcause
  csrr t2,  0x7d0        # miex
  sw   t1,  0x90(sp)                # mtval
  csrr t1,  0x7d1        # mtvecx
  sw   t2,  0x94(sp)                # miex
  sw   t1,  0x98(sp)                # mtvecx

  csrr t0,  0xf14
  sll  t0, t0, 8
  li   t1, ARCHI_TCLS_ADDR
  add  t0, t0, t1
  sw   sp, 0x00(t0)     # Store sp to TCLS sp reg -> subsequent mismatches will trigger restart of resynchronization

  nop
  nop
  nop

  .global pos_TCLS_reload
pos_TCLS_reload:

  csrr t0,  0xf14
  sll  t0, t0, 8
  li   t1, ARCHI_TCLS_ADDR
  add  t0, t0, t1
  lw   sp,  0x00(t0)

  lw   t1,  0x78(sp)                # mepc
  lw   t2,  0x7C(sp)                # mstatus
  csrw 0x341,  t1        # mepc
  lw   t1,  0x80(sp)                # mie
  csrw 0x300,  t2        # mstatus
  lw   t2,  0x84(sp)                # mtvec
  csrw 0x304,  t1        # mie
  lw   t1,  0x88(sp)                # mscratch
  csrw 0x305,  t2        # mtvec
  lw   t2,  0x8C(sp)                # mcause
  csrw 0x340,  t1        # mscratch
  lw   t1,  0x90(sp)                # mtval
  csrw 0x342,  t2        # mcause
  lw   t2,  0x94(sp)                # miex
  csrw 0x343,  t1        # mtval
  lw   t1,  0x98(sp)                # mtvecx
  csrw 0x7d0,  t2        # miex
  csrw 0x7d1,  t1        # mtvecx

  # zero not loaded as hardwired    #  x0
  lw   ra,  0x00(sp)                #  x1
  # sp loaded from TCLS above       #  x2
  lw   gp,  0x04(sp)                #  x3
  lw   tp,  0x08(sp)                #  x4
  # t0 loaded below, as this reg stores pointer to TCLS unit
  # lw   t0,  0x0C(sp)              #  x5
  lw   t1,  0x10(sp)                #  x6
  lw   t2,  0x14(sp)                #  x7
  lw   x8,  0x18(sp)                #  fp
  lw   s1,  0x1C(sp)                #  x9
  lw   a0,  0x20(sp)                # x10
  lw   a1,  0x24(sp)                # x11
  lw   a2,  0x28(sp)                # x12
  lw   a3,  0x2C(sp)                # x13
  lw   a4,  0x30(sp)                # x14
  lw   a5,  0x34(sp)                # x15
  lw   a6,  0x38(sp)                # x16
  lw   a7,  0x3C(sp)                # x17
  lw   s2,  0x40(sp)                # x18
  lw   s3,  0x44(sp)                # x19
  lw   s4,  0x48(sp)                # x20
  lw   s5,  0x4C(sp)                # x21
  lw   s6,  0x50(sp)                # x22
  lw   s7,  0x54(sp)                # x23
  lw   s8,  0x58(sp)                # x24
  lw   s9,  0x5C(sp)                # x25
  lw   s10, 0x60(sp)                # x26
  lw   s11, 0x64(sp)                # x27
  lw   t3,  0x68(sp)                # x28
  lw   t4,  0x6C(sp)                # x29
  lw   t5,  0x70(sp)                # x30
  lw   t6,  0x74(sp)                # x31

  sw   zero, 0x00(t0)     # Clear TCLS sp reg -> subsequent mismatches will trigger unload and reload
  lw   t0,  0x0C(sp)                #  x5
  add  sp, sp, 0xA0
  mret

