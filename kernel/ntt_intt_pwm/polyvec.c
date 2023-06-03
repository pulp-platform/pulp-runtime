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
	int32_t Din[128], Dout[128];
    poly vector;
	uint32_t concatenated;

    vector = r->vec[0];

	for (i = 0; i <= 254; i += 2) {
        concatenated = 0;
        concatenated = ((int32_t)vector.coeffs[i] << 16) | ((int32_t)vector.coeffs[i + 1] & 0xFFFF);
        Din[i/2] = concatenated;
    }

    for (i = 0; i < 128; i++) {
        printf("%08x-", Din[i]);
    }

	printf("\nNTT accelerator starts working!\n");
	KYBER_poly_ntt(Din[256], Dout[256]);

	for (i = 0; i < 256; i++) {
        printf("%hx-", Dout[i]);
    }
	printf("NTT accelerator ends working!\n");

	/*printf("after NTT:\n");
	for (i = 0; i < 256; i++) {	 
		printbytes(Dout[i], 4);
	}*/

	

}

