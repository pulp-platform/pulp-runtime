#include <stdint.h>

//void reset_ntt_intt_pwm();
void set_clear_pwm();
void set_input_pwm(uint32_t Din);

void trigger_input1_pwm(void);
void trigger_input2_pwm(void);

void trigger_pwm(void);

void poll_done_pwm(void);

uint32_t set_output_pwm();

/***MAIN**/
void KYBER_poly_pwm(uint32_t Dout[128], uint32_t Din1[128], uint32_t Din2[128]);
