#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include <stdint.h>
#include <stdio.h>

/*************************************************
* Name:        PQCLEAN_KYBER768_CLEAN_polyvec_compress
*
* Description: Compress and serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYVECCOMPRESSEDBYTES)
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void PQCLEAN_KYBER768_CLEAN_polyvec_compress(uint8_t r[KYBER_POLYVECCOMPRESSEDBYTES], const polyvec *a) {
    unsigned int i, j, k;

    uint16_t t[4];
    for (i = 0; i < KYBER_K; i++) {
        for (j = 0; j < KYBER_N / 4; j++) {
            for (k = 0; k < 4; k++) {
                t[k]  = a->vec[i].coeffs[4 * j + k];
                t[k] += ((int16_t)t[k] >> 15) & KYBER_Q;
                t[k]  = ((((uint32_t)t[k] << 10) + KYBER_Q / 2) / KYBER_Q) & 0x3ff;
            }

            r[0] = (uint8_t)(t[0] >> 0);
            r[1] = (uint8_t)((t[0] >> 8) | (t[1] << 2));
            r[2] = (uint8_t)((t[1] >> 6) | (t[2] << 4));
            r[3] = (uint8_t)((t[2] >> 4) | (t[3] << 6));
            r[4] = (uint8_t)(t[3] >> 2);
            r += 5;
        }
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER768_CLEAN_polyvec_decompress
*
* Description: De-serialize and decompress vector of polynomials;
*              approximate inverse of PQCLEAN_KYBER768_CLEAN_polyvec_compress
*
* Arguments:   - polyvec *r:       pointer to output vector of polynomials
*              - const uint8_t *a: pointer to input byte array
*                                  (of length KYBER_POLYVECCOMPRESSEDBYTES)
**************************************************/
void PQCLEAN_KYBER768_CLEAN_polyvec_decompress(polyvec *r, const uint8_t a[KYBER_POLYVECCOMPRESSEDBYTES]) {
    unsigned int i, j, k;

    uint16_t t[4];
    for (i = 0; i < KYBER_K; i++) {
        for (j = 0; j < KYBER_N / 4; j++) {
            t[0] = (a[0] >> 0) | ((uint16_t)a[1] << 8);
            t[1] = (a[1] >> 2) | ((uint16_t)a[2] << 6);
            t[2] = (a[2] >> 4) | ((uint16_t)a[3] << 4);
            t[3] = (a[3] >> 6) | ((uint16_t)a[4] << 2);
            a += 5;

            for (k = 0; k < 4; k++) {
                r->vec[i].coeffs[4 * j + k] = ((uint32_t)(t[k] & 0x3FF) * KYBER_Q + 512) >> 10;
            }
        }
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER768_CLEAN_polyvec_tobytes
*
* Description: Serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYVECBYTES)
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void PQCLEAN_KYBER768_CLEAN_polyvec_tobytes(uint8_t r[KYBER_POLYVECBYTES], const polyvec *a) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER768_CLEAN_poly_tobytes(r + i * KYBER_POLYBYTES, &a->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER768_CLEAN_polyvec_frombytes
*
* Description: De-serialize vector of polynomials;
*              inverse of PQCLEAN_KYBER768_CLEAN_polyvec_tobytes
*
* Arguments:   - uint8_t *r:       pointer to output byte array
*              - const polyvec *a: pointer to input vector of polynomials
*                                  (of length KYBER_POLYVECBYTES)
**************************************************/
void PQCLEAN_KYBER768_CLEAN_polyvec_frombytes(polyvec *r, const uint8_t a[KYBER_POLYVECBYTES]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER768_CLEAN_poly_frombytes(&r->vec[i], a + i * KYBER_POLYBYTES);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER768_CLEAN_polyvec_ntt
*
* Description: Apply forward NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void PQCLEAN_KYBER768_CLEAN_polyvec_ntt(polyvec *r) {
    unsigned int i, k2;
	  uint32_t Din[128], Dout[128];
    poly vector;
	  uint32_t concatenated;


    for (i = 0; i < KYBER_K; i++) {
        vector = r->vec[i];
    
         for (int k1 = 0; k1 <= 254; k1 += 2) {
            concatenated = 0;
            concatenated = ((uint32_t)vector.coeffs[k1] << 16) | ((uint32_t)vector.coeffs[k1 + 1] & 0xFFFF);
            Din[k1 / 2] = concatenated;
        }
        
        /*printf("\npoly_INTT INPUT on 32bit *******************************\n");
        for (int j = 0; j < 128; j++) {
            printf("%08x-", Din[j]);
        }
        printf("\n");
        printf("\nNTT accelerator starts working!\n");*/
        
        KYBER_poly_ntt(Din, Dout);
        /*printf("\nNTT accelerator ends working!\n");
       
        printf("\nOUTPUT from NTT - DOUT [%d]\n", i);
        for (int i = 0; i < 128; i++) {	 
		      printf("%08x-",Dout[i]);
	      }*/
    
        for (k2 = 0; k2 < 128; k2+=2) {
          uint32_t value1 = Dout[k2];
          uint16_t msb1 = (value1 >> 16) & 0xFFFF;
          uint16_t lsb1 = value1 & 0xFFFF;

          uint32_t value2 = Dout[k2+1];
          uint16_t msb2 = (value2 >> 16) & 0xFFFF;
          uint16_t lsb2 = value2 & 0xFFFF;
          
   
          // Assign the MSB and LSB to the corresponding elements in r
          r->vec[i].coeffs[2 * k2] = msb1; // Assign MSB1
          r->vec[i].coeffs[2 * k2 + 1] = msb2; // Assign MSB2
          r->vec[i].coeffs[2 * k2 + 2] = lsb1; // Assign LSB1
          r->vec[i].coeffs[2 * k2 + 3] = lsb2; // Assign LSB2
          
        }
        
        /*printf("\nOUTPUT from polyvec [%d]\n", i);
        for (int k3 = 0; k3 < 256; k3++) {
            printf("%04x-", r->vec[i].coeffs[k3]);
        }*/
     
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER768_CLEAN_polyvec_invntt_tomont
*
* Description: Apply inverse NTT to all elements of a vector of polynomials
*              and multiply by Montgomery factor 2^16
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void PQCLEAN_KYBER768_CLEAN_polyvec_invntt_tomont(polyvec *r) {
    /*unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER768_CLEAN_poly_invntt_tomont(&r->vec[i]);
    }*/
    
    unsigned int i, k2;
	  uint32_t Din[128], Dout[128];
    poly vector;
	  uint32_t concatenated1, concatenated2;


    for (i = 0; i < KYBER_K; i++) {
        vector = r->vec[i];
        

        for (int k1 = 0; k1 <= 254; k1 += 4) {
            concatenated1 = 0;
            concatenated2 = 0;
            concatenated1 = ((uint32_t)vector.coeffs[k1] << 16) | ((uint32_t)vector.coeffs[k1 + 2] & 0xFFFF);
            concatenated2 = ((uint32_t)vector.coeffs[k1+1] << 16) | ((uint32_t)vector.coeffs[k1 + 3] & 0xFFFF);
            Din[k1 / 2] = concatenated1;
            Din[k1 / 2 + 1] = concatenated2;
        }
        
        printf("\npoly_INTT INPUT on 32bit *******************************\n");
        for (int j = 0; j < 128; j++) {
            printf("%08x-", Din[j]);
        }
        printf("\n");
        
     
        printf("\nINTT accelerator starts working!\n");
        KYBER_poly_intt(Din, Dout);
        printf("\nINTT accelerator ends working!\n");
       
        printf("\nOUTPUT from INVNTT - DOUT [%d]\n", i);
        for (int i = 0; i < 128; i++) {	 
		      printf("%08x-",Dout[i]);
	      }
    
        for (k2 = 0; k2 < 128; k2++) {
          uint32_t value = Dout[k2];
          uint16_t msb = (value >> 16) & 0xFFFF;
          uint16_t lsb = value & 0xFFFF;

          // Assign the MSB and LSB to the corresponding elements in r
          r->vec[i].coeffs[k2] = msb; // Assign MSB
          r->vec[i].coeffs[k2 + 128] = lsb; // Assign LSB

          
        }
        
        printf("\nOUTPUT INVNTT from polyvec [%d]\n", i);
        for (int k3 = 0; k3 < 256; k3++) {
            printf("%04x-", r->vec[i].coeffs[k3]);
        }
     
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER768_CLEAN_polyvec_basemul_acc_montgomery
*
* Description: Multiply elements of a and b in NTT domain, accumulate into r,
*              and multiply by 2^-16.
*
* Arguments: - poly *r: pointer to output polynomial
*            - const polyvec *a: pointer to first input vector of polynomials
*            - const polyvec *b: pointer to second input vector of polynomials
**************************************************/
void PQCLEAN_KYBER768_CLEAN_polyvec_basemul_acc_montgomery(poly *r, const polyvec *a, const polyvec *b) {
    unsigned int i;
    poly t;
    uint16_t DIN_1, DIN_2, DOUT, DOUTa;

    /*printf("\nDIN1:\n");
    for (int j = 0; j < KYBER_N; j++) {
        DIN_1 = a[0].vec[0].coeffs[j];
        printf("%02X-", DIN_1);
    }
    printf("\nDIN2:\n");
    for (int j = 0; j < KYBER_N; j++) {
        DIN_2 = b[0].vec[0].coeffs[j];
        printf("%02X-", DIN_2);
    }*/

   PQCLEAN_KYBER768_CLEAN_poly_basemul_montgomery(r, &a->vec[0], &b->vec[0]);
   
   /*printf("\nDOT:\n");
   for (int j = 0; j < KYBER_N; j++) {
        DOUT = r[0].coeffs[j];
        printf("%02X-", DOUT);
    }*/
    
    
    for (i = 1; i < KYBER_K; i++) {
        PQCLEAN_KYBER768_CLEAN_poly_basemul_montgomery(&t, &a->vec[i], &b->vec[i]);
        /*printf("\nDOT:\n");
         for (int j = 0; j < KYBER_N; j++) {
           DOUTa = t.coeffs[j];
           printf("%02X-", DOUTa);
        }*/
        PQCLEAN_KYBER768_CLEAN_poly_add(r, r, &t);
    }

    PQCLEAN_KYBER768_CLEAN_poly_reduce(r);
}

/*************************************************
* Name:        PQCLEAN_KYBER768_CLEAN_polyvec_reduce
*
* Description: Applies Barrett reduction to each coefficient
*              of each element of a vector of polynomials;
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - polyvec *r: pointer to input/output polynomial
**************************************************/
void PQCLEAN_KYBER768_CLEAN_polyvec_reduce(polyvec *r) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER768_CLEAN_poly_reduce(&r->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER768_CLEAN_polyvec_add
*
* Description: Add vectors of polynomials
*
* Arguments: - polyvec *r: pointer to output vector of polynomials
*            - const polyvec *a: pointer to first input vector of polynomials
*            - const polyvec *b: pointer to second input vector of polynomials
**************************************************/
void PQCLEAN_KYBER768_CLEAN_polyvec_add(polyvec *r, const polyvec *a, const polyvec *b) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER768_CLEAN_poly_add(&r->vec[i], &a->vec[i], &b->vec[i]);
    }
}
