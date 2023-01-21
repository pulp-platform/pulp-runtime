#include <stdint.h>

void set_input(uint32_t* Din);
void trigger_keccak(void);
void poll_done(void);
void get_result(uint32_t* Dout);
void KeccakF1600_StatePermute(uint32_t Din[50], uint32_t Dout[50]);

