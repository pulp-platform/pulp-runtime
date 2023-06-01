#ifndef PQCLEAN_KYBER768_CLEAN_INDCPA_H
#define PQCLEAN_KYBER768_CLEAN_INDCPA_H
#include "params.h"
#include "polyvec.h"
#include <stdint.h>


void PQCLEAN_KYBER768_CLEAN_indcpa_keypair(uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES],uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES]);


#endif
