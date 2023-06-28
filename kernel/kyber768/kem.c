#include "indcpa.h"
#include "kem.h"
#include "params.h"
#include "randombytes.h"
#include "symmetric.h"
#include "verify.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

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
    for (i = 0; i < KYBER_INDCPA_PUBLICKEYBYTES; i++) {
        sk[i + KYBER_INDCPA_SECRETKEYBYTES] = pk[i];
    }
    hash_h(sk + KYBER_SECRETKEYBYTES - 2 * KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);
    /* Value z for pseudo-random output on reject */
    randombytes(sk + KYBER_SECRETKEYBYTES - KYBER_SYMBYTES, KYBER_SYMBYTES);
    return 0;
}

/*************************************************
* Name:        PQCLEAN_KYBER768_CLEAN_crypto_kem_enc
*
* Description: Generates cipher text and shared
*              secret for given public key
*
* Arguments:   - uint8_t *ct: pointer to output cipher text
*                (an already allocated array of KYBER_CIPHERTEXTBYTES bytes)
*              - uint8_t *ss: pointer to output shared secret
*                (an already allocated array of KYBER_SSBYTES bytes)
*              - const uint8_t *pk: pointer to input public key
*                (an already allocated array of KYBER_PUBLICKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
int PQCLEAN_KYBER768_CLEAN_crypto_kem_enc(uint8_t *ct,
        uint8_t *ss,
        const uint8_t *pk) {
    uint8_t buf_enc[2 * KYBER_SYMBYTES];
    /* Will contain key, coins */
    uint8_t kr[2 * KYBER_SYMBYTES];
    uint8_t A;

    printf("\nKYBER_SYMBYTES: %d:\n",KYBER_SYMBYTES );
    //randombytes(buf_enc, KYBER_SYMBYTES);
    
    buf_enc[0] = 0xBA;
    buf_enc[1] = 0xC5;
    buf_enc[2] = 0xBA;
    buf_enc[3] = 0x88;
    buf_enc[4] = 0x1D;
    buf_enc[5] = 0xD3;
    buf_enc[6] = 0x5C;
    buf_enc[7] = 0x59;
    buf_enc[8] = 0x71;
    buf_enc[9] = 0x96;
    buf_enc[10] = 0x70;
    buf_enc[11] = 0x00;
    buf_enc[12] = 0x46;
    buf_enc[13] = 0x92;
    buf_enc[14] = 0xD6;
    buf_enc[15] = 0x75;
    buf_enc[16] = 0xB8;
    buf_enc[17] = 0x3C;
    buf_enc[18] = 0x98;
    buf_enc[19] = 0xDB;
    buf_enc[20] = 0x6A;
    buf_enc[21] = 0x0E;
    buf_enc[22] = 0x55;
    buf_enc[23] = 0x80;
    buf_enc[24] = 0x0B;
    buf_enc[25] = 0xAF;
    buf_enc[26] = 0xEB;
    buf_enc[27] = 0x7E;
    buf_enc[28] = 0x70;
    buf_enc[29] = 0x49;
    buf_enc[30] = 0x1B;
    buf_enc[31] = 0xF4;
    
    /*printf("\nRANDOM buf_enc:\n");
    for (int j = 0; j < 64; j++) {
        A = buf_enc[j];
        printf("%02X-", A);
    }*/
    /* Don't release system RNG output */
    hash_h(buf_enc, buf_enc, KYBER_SYMBYTES);

    /* Multitarget countermeasure for coins + contributory KEM */
    hash_h(buf_enc + KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);
    hash_g(kr, buf_enc, 2 * KYBER_SYMBYTES);


    /* coins are in kr+KYBER_SYMBYTES */
    PQCLEAN_KYBER768_CLEAN_indcpa_enc(ct, buf_enc, pk, kr + KYBER_SYMBYTES);
    for (int j = 0; j < KYBER_CIPHERTEXTBYTES; j++) {
        printf("%02X-", ct[j]);
    }
    

    /* overwrite coins in kr with H(c) */
    hash_h(kr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);
    /* hash concatenation of pre-k and H(c) to k */
    kdf(ss, kr, 2 * KYBER_SYMBYTES);
    return 0;
}

/*************************************************
* Name:        PQCLEAN_KYBER768_CLEAN_crypto_kem_dec
*
* Description: Generates shared secret for given
*              cipher text and private key
*
* Arguments:   - uint8_t *ss: pointer to output shared secret
*                (an already allocated array of KYBER_SSBYTES bytes)
*              - const uint8_t *ct: pointer to input cipher text
*                (an already allocated array of KYBER_CIPHERTEXTBYTES bytes)
*              - const uint8_t *sk: pointer to input private key
*                (an already allocated array of KYBER_SECRETKEYBYTES bytes)
*
* Returns 0.
*
* On failure, ss will contain a pseudo-random value.
**************************************************/
int PQCLEAN_KYBER768_CLEAN_crypto_kem_dec(uint8_t *ss,
        const uint8_t *ct,
        const uint8_t *sk) {
    size_t i;
    int fail;
    uint8_t buf[2 * KYBER_SYMBYTES];
    /* Will contain key, coins */
    uint8_t kr[2 * KYBER_SYMBYTES];
    uint8_t cmp[KYBER_CIPHERTEXTBYTES];
    const uint8_t *pk = sk + KYBER_INDCPA_SECRETKEYBYTES;

    PQCLEAN_KYBER768_CLEAN_indcpa_dec(buf, ct, sk);

    /* Multitarget countermeasure for coins + contributory KEM */
    for (i = 0; i < KYBER_SYMBYTES; i++) {
        buf[KYBER_SYMBYTES + i] = sk[KYBER_SECRETKEYBYTES - 2 * KYBER_SYMBYTES + i];
    }
    hash_g(kr, buf, 2 * KYBER_SYMBYTES);

    /* coins are in kr+KYBER_SYMBYTES */
    PQCLEAN_KYBER768_CLEAN_indcpa_enc(cmp, buf, pk, kr + KYBER_SYMBYTES);

    fail = PQCLEAN_KYBER768_CLEAN_verify(ct, cmp, KYBER_CIPHERTEXTBYTES);

    /* overwrite coins in kr with H(c) */
    hash_h(kr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);

    /* Overwrite pre-k with z on re-encryption failure */
    PQCLEAN_KYBER768_CLEAN_cmov(kr, sk + KYBER_SECRETKEYBYTES - KYBER_SYMBYTES, KYBER_SYMBYTES, (uint8_t)fail);

    /* hash concatenation of pre-k and H(c) to k */
    kdf(ss, kr, 2 * KYBER_SYMBYTES);
    return 0;
}
