#include "indcpa.h"
#include "kem.h"
#include "params.h"
#include "randombytes.h"
#include "symmetric.h"
#include "verify.h"
#include <stddef.h>
#include <stdint.h>

/*************************************************
* Name:        PQCLEAN_KYBER768_CLEAN_crypto_kem_keypair
*
* Description: Generates public and private key
*              for CCA-secure Kyber key encapsulation mechanism
*
* Arguments:   - uint8_t *pk: pointer to output public key
*                (an already allocated array of KYBER_PUBLICKEYBYTES bytes)
*              - uint8_t *sk: pointer to output private key
*                (an already allocated array of KYBER_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
int PQCLEAN_KYBER768_CLEAN_crypto_kem_keypair(uint8_t *pk,
        uint8_t *sk) {
    size_t i;
    PQCLEAN_KYBER768_CLEAN_indcpa_keypair(pk, sk);
    
    return 0;
}

