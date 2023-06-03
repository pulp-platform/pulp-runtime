// Generated register defines for ntt_intt_pwm

#ifndef _NTT_INTT_PWM_REG_DEFS_
#define _NTT_INTT_PWM_REG_DEFS_

#ifdef __cplusplus
extern "C" {
#endif

#define NTT_INTT_PWM0_BASE_ADDR 0x1A401000
// auto added parameter
#define NTT_INTT_PWM_PARAM_DIN 1

// auto added parameter
#define NTT_INTT_PWM_PARAM_DOUT 1

// Register width
#define NTT_INTT_PWM_PARAM_REG_WIDTH 32

// Common Interrupt Offsets

// Subword of input of ntt_intt_pwm module (common parameters)
#define NTT_INTT_PWM_DIN_DIN_FIELD_WIDTH 32
#define NTT_INTT_PWM_DIN_DIN_FIELDS_PER_REG 1
#define NTT_INTT_PWM_DIN_MULTIREG_COUNT 1

// Subword of input of ntt_intt_pwm module
#define NTT_INTT_PWM_DIN(id) (NTT_INTT_PWM##id##_BASE_ADDR + 0x0)
#define NTT_INTT_PWM_DIN_REG_OFFSET 0x0

// Subword of results. (common parameters)
#define NTT_INTT_PWM_DOUT_DOUT_FIELD_WIDTH 32
#define NTT_INTT_PWM_DOUT_DOUT_FIELDS_PER_REG 1
#define NTT_INTT_PWM_DOUT_MULTIREG_COUNT 1

// Subword of results.
#define NTT_INTT_PWM_DOUT(id) (NTT_INTT_PWM##id##_BASE_ADDR + 0x4)
#define NTT_INTT_PWM_DOUT_REG_OFFSET 0x4

// Controls signals of ntt_intt_pwm
#define NTT_INTT_PWM_CTRL(id) (NTT_INTT_PWM##id##_BASE_ADDR + 0x8)
#define NTT_INTT_PWM_CTRL_REG_OFFSET 0x8
#define NTT_INTT_PWM_CTRL_LOAD_A_F 0
#define NTT_INTT_PWM_CTRL_LOAD_A_I 1
#define NTT_INTT_PWM_CTRL_LOAD_B_F 2
#define NTT_INTT_PWM_CTRL_LOAD_B_I 3
#define NTT_INTT_PWM_CTRL_READ_A 4
#define NTT_INTT_PWM_CTRL_READ_B 5
#define NTT_INTT_PWM_CTRL_START_AB 6
#define NTT_INTT_PWM_CTRL_START_NTT 7
#define NTT_INTT_PWM_CTRL_START_PWM 8
#define NTT_INTT_PWM_CTRL_START_INTT 9

//  Contains status information about ntt_intt_pwm
#define NTT_INTT_PWM_STATUS(id) (NTT_INTT_PWM##id##_BASE_ADDR + 0xc)
#define NTT_INTT_PWM_STATUS_REG_OFFSET 0xc
#define NTT_INTT_PWM_STATUS_STATUS 0

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _NTT_INTT_PWM_REG_DEFS_
// End generated register defines for ntt_intt_pwm
