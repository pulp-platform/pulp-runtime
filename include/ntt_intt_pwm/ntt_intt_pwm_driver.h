#include <stdint.h>

void set_input_ntt_intt_pwm(void);
void wait_for_input(void);
void trigger_ntt(void);
void poll_done_ntt_intt_pwm(void);
//void get_result_ntt_intt_pwm(int16_t* Dout);
void KYBER_poly_ntt(int16_t Din[256], int16_t Dout[256]);

