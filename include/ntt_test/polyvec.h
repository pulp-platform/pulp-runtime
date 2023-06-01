#ifndef PQCLEAN_KYBER768_CLEAN_POLYVEC_H
#define PQCLEAN_KYBER768_CLEAN_POLYVEC_H
#include "params.h"
#include "poly.h"
#include <stdint.h>

typedef struct {
    poly vec[KYBER_K];
} polyvec;

void PQCLEAN_KYBER768_CLEAN_polyvec_ntt(polyvec *r);

#endif
