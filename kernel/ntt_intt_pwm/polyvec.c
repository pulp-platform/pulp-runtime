#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

static void printbytes(const uint8_t *x, size_t xlen) {
    size_t i;
    for (i = 0; i < xlen; i++) {
        printf("%02x", x[i]);
    }
    printf("\n");
    }
/*************************************************
* Name:        PQCLEAN_KYBER768_CLEAN_polyvec_ntt
*
* Description: Apply forward NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void PQCLEAN_KYBER768_CLEAN_polyvec_ntt(polyvec *r) {
    unsigned int i;
	int16_t Din[256], Dout[256];

	for (i = 0; i < 256; i++) {	 
		Din[i] = &r->vec[0].coeffs[i];
	}

	printf("NTT accelerator starts working!\n");
	KYBER_poly_ntt(Din[256], Dout[256]);
	printf("NTT accelerator ends working!\n");

	/*printf("after NTT:\n");
	for (i = 0; i < 256; i++) {	 
		printbytes(Dout[i], 4);
	}*/

	

}

