#include <stdint.h>


void trigger_input_ntt(void);

void set_input_ntt(uint32_t Din);

void trigger_ntt(void);

void poll_done_ntt_intt_pwm(void);

void trigger_output_ntt(void);

void set_output_ntt(uint32_t Dout);

/***MAIN**/
void KYBER_poly_ntt(uint32_t Din[128], uint32_t Dout[128]);

