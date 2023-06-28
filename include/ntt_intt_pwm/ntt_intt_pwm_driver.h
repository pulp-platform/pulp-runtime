#include <stdint.h>

void set_clear_ntt(void);

void set_input_ntt(uint32_t Din);

void trigger_input_ntt(void);

void trigger_ntt(void);

void poll_done_ntt_intt_pwm(void);

uint32_t set_output_ntt();

/***MAIN**/
void KYBER_poly_ntt(uint32_t Din[128], uint32_t Dout[128]);

