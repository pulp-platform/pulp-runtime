#ifndef PQCLEAN_KYBER768_CLEAN_POLY_H
#define PQCLEAN_KYBER768_CLEAN_POLY_H
#include "params.h"
#include <stdint.h>

/*
 * Elements of R_q = Z_q[X]/(X^n + 1). Represents polynomial
 * coeffs[0] + X*coeffs[1] + X^2*xoeffs[2] + ... + X^{n-1}*coeffs[n-1]
 */
typedef struct {
    int16_t coeffs[KYBER_N];
} poly;


void PQCLEAN_KYBER768_CLEAN_poly_getnoise_eta1(poly *r, const uint8_t seed[KYBER_SYMBYTES], uint8_t nonce);



#endif
