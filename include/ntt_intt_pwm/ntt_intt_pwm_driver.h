#include <stdint.h>

void set_input_ntt_intt_pwm(void);

void wait_for_input(int32_t Din[128]);

void trigger_ntt(void);

void set_output_ntt_intt_pwm(void);

void wait_for_output(int32_t Dout[128]);

/***MAIN**/
void KYBER_poly_ntt(int32_t Din[128], int32_t Dout[128]);

