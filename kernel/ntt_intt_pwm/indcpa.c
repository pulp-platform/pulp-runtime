#include "indcpa.h"
#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include "randombytes.h"
#include "symmetric.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


/*************************************************
* Name:        PQCLEAN_KYBER768_CLEAN_indcpa_keypair
*
* Description: Generates public and private key for the CPA-secure
*              public-key encryption scheme underlying Kyber
*
* Arguments:   - uint8_t *pk: pointer to output public key
*                             (of length KYBER_INDCPA_PUBLICKEYBYTES bytes)
*              - uint8_t *sk: pointer to output private key
                              (of length KYBER_INDCPA_SECRETKEYBYTES bytes)
**************************************************/
void PQCLEAN_KYBER768_CLEAN_indcpa_keypair(uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES],
        uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES]) {
    unsigned int i;
    uint8_t buf[2 * KYBER_SYMBYTES];
    const uint8_t *publicseed = buf;
    const uint8_t *noiseseed = buf + KYBER_SYMBYTES;
    uint8_t nonce = 0;
    polyvec skpv;
	uint16_t A;

    
    PQCLEAN_KYBER768_CLEAN_poly_getnoise_eta1(&skpv.vec[0], noiseseed, nonce++);
	printf("Getnoise executed!\n");
	
	/*for (int j=0; j < KYBER_N; j++){
		A = skpv.vec[0].coeffs[j];
		printf("%04X-",A);	
	}*/
    PQCLEAN_KYBER768_CLEAN_polyvec_ntt(&skpv);
 
}

