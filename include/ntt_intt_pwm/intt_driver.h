#include <stdint.h>

void set_input_intt(uint32_t Din);

void trigger_input_intt(void);

void trigger_intt(void);

void poll_done_intt(void);

uint32_t set_output_intt();

/***MAIN**/
void KYBER_poly_intt(uint32_t Din[128], uint32_t Dout[128]);
