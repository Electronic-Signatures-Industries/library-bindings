
/*
 * Copyright (c) 2012-2020 MIRACL UK Ltd.
 *
 * This file is part of MIRACL Core
 * (see https://github.com/miracl/core).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Test and benchmark elliptic curve and RSA functions
    First build core.a from build_ec batch file
    gcc -O3 benchtest_ec.c core.a -o benchtest_ec.exe
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rsa_RSA2048.h"
#include "ecp_ED25519.h"
#include "pair_BN254.h"

#if CHUNK==32 || CHUNK==64
#include "ecp_NIST256.h"
#include "ecp_GOLDILOCKS.h"
#include "pair_BLS12383.h"
#include "pair4_BLS24479.h"
#include "pair8_BLS48556.h"
#endif

#define MIN_TIME 10.0
#define MIN_ITERS 10

using namespace core;

int ed25519(csprng *RNG)
{
    using namespace ED25519;
    using namespace ED25519_BIG;
    using namespace ED25519_FP;

    int i, iterations;
    clock_t start;
    double elapsed;
    ECP EP, EG;
    BIG s, r, x, y;
    FP rw;
    printf("\nTesting/Timing ED25519 ECC\n");

#if CURVETYPE_ED25519==WEIERSTRASS
    printf("Weierstrass parameterization\n");
#endif
#if CURVETYPE_ED25519==EDWARDS
    printf("Edwards parameterization\n");
#endif
#if CURVETYPE_ED25519==MONTGOMERY
    printf("Montgomery parameterization\n");
#endif


#if MODTYPE_F25519 == PSEUDO_MERSENNE
    printf("Pseudo-Mersenne Modulus\n");
#endif

#if MODTYPE_F25519 == GENERALISED_MERSENNE
    printf("Generalised-Mersenne Modulus\n");
#endif

#if MODTYPE_F25519 == MONTGOMERY_FRIENDLY
    printf("Montgomery Friendly Modulus\n");
#endif

#if MODTYPE_F25519 == NOT_SPECIAL
    printf("Not special Modulus\n");
#endif


#if CHUNK==16
    printf("16-bit Build\n");
#endif
#if CHUNK==32
    printf("32-bit Build\n");
#endif
#if CHUNK==64
    printf("64-bit Build\n");
#endif

    ECP_generator(&EG);
    BIG_rcopy(r, CURVE_Order);

    FP_rand(&rw,RNG);

    ECP_map2point(&EP,&rw);
    ECP_cfp(&EP);

    if (ECP_isinf(&EP))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }
    
    ECP_mul(&EP,r);
    if (!ECP_isinf(&EP))
    {
        printf("EP= "); ECP_output(&EP); printf("\n");
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }

    BIG_randtrunc(s, r, 2 * CURVE_SECURITY_ED25519, RNG);

    ECP_copy(&EP, &EG);
    ECP_mul(&EP, r);

    if (!ECP_isinf(&EP))
    {
        printf("FAILURE - rG!=O\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        ECP_copy(&EP, &EG);
        ECP_mul(&EP, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("EC  mul - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    return 0;
}

#if CHUNK==32 || CHUNK==64
int nist256(csprng *RNG)
{
    using namespace NIST256;
    using namespace NIST256_BIG;
    using namespace NIST256_FP;

    int i, iterations;
    clock_t start;
    double elapsed;
    ECP EP, EG;
    BIG s, r, x, y;
    FP rw;
    printf("\nTesting/Timing NIST256 ECC\n");

#if CURVETYPE_NIST256==WEIERSTRASS
    printf("Weierstrass parameterization\n");
#endif
#if CURVETYPE_NIST256==EDWARDS
    printf("Edwards parameterization\n");
#endif
#if CURVETYPE_NIST256==MONTGOMERY
    printf("Montgomery parameterization\n");
#endif


#if MODTYPE_NIST256 == PSEUDO_MERSENNE
    printf("Pseudo-Mersenne Modulus\n");
#endif

#if MODTYPE_NIST256 == GENERALISED_MERSENNE
    printf("Generalised-Mersenne Modulus\n");
#endif

#if MODTYPE_NIST256 == MONTGOMERY_FRIENDLY
    printf("Montgomery Friendly Modulus\n");
#endif

#if MODTYPE_NIST256 == NOT_SPECIAL
    printf("Not special Modulus\n");
#endif

#if CHUNK==16
    printf("16-bit Build\n");
#endif
#if CHUNK==32
    printf("32-bit Build\n");
#endif
#if CHUNK==64
    printf("64-bit Build\n");
#endif

    ECP_generator(&EG);
    BIG_rcopy(r, CURVE_Order);

    FP_rand(&rw,RNG);
    ECP_map2point(&EP,&rw);
    ECP_cfp(&EP);

    if (ECP_isinf(&EP))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }
    ECP_mul(&EP,r);
    if (!ECP_isinf(&EP))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }

    BIG_randtrunc(s, r, 2 * CURVE_SECURITY_NIST256, RNG);

    ECP_copy(&EP, &EG);
    ECP_mul(&EP, r);

    if (!ECP_isinf(&EP))
    {
        printf("FAILURE - rG!=O\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        ECP_copy(&EP, &EG);
        ECP_mul(&EP, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("EC  mul - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    return 0;
}

int goldilocks(csprng *RNG)
{
    using namespace GOLDILOCKS;
    using namespace GOLDILOCKS_BIG;
    using namespace GOLDILOCKS_FP;

    int i, iterations;
    clock_t start;
    double elapsed;
    ECP EP, EG;
    BIG s, r, x, y;
    FP rw;
    printf("\nTesting/Timing GOLDILOCKS ECC\n");

#if CURVETYPE_GOLDILOCKS==WEIERSTRASS
    printf("Weierstrass parameterization\n");
#endif
#if CURVETYPE_GOLDILOCKS==EDWARDS
    printf("Edwards parameterization\n");
#endif
#if CURVETYPE_GOLDILOCKS==MONTGOMERY
    printf("Montgomery parameterization\n");
#endif

#if MODTYPE_GOLDILOCKS == PSEUDO_MERSENNE
    printf("Pseudo-Mersenne Modulus\n");
#endif

#if MODTYPE_GOLDILOCKS == GENERALISED_MERSENNE
    printf("Generalised-Mersenne Modulus\n");
#endif

#if MODTYPE_GOLDILOCKS == MONTGOMERY_FRIENDLY
    printf("Montgomery Friendly Modulus\n");
#endif

#if MODTYPE_GOLDILOCKS == NOT_SPECIAL
    printf("Not special Modulus\n");
#endif

#if CHUNK==16
    printf("16-bit Build\n");
#endif
#if CHUNK==32
    printf("32-bit Build\n");
#endif
#if CHUNK==64
    printf("64-bit Build\n");
#endif

    ECP_generator(&EG);
    BIG_rcopy(r, CURVE_Order);
    FP_rand(&rw,RNG);
    ECP_map2point(&EP,&rw);
    ECP_cfp(&EP);

    if (ECP_isinf(&EP))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }
    ECP_mul(&EP,r);
    if (!ECP_isinf(&EP))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }

    BIG_randtrunc(s, r, 2 * CURVE_SECURITY_GOLDILOCKS, RNG);

    ECP_copy(&EP, &EG);
    ECP_mul(&EP, r);

    if (!ECP_isinf(&EP))
    {
        printf("FAILURE - rG!=O\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        ECP_copy(&EP, &EG);
        ECP_mul(&EP, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("EC  mul - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    return 0;
}
#endif

int bn254(csprng *RNG)
{
    using namespace BN254;
    using namespace BN254_FP;
    using namespace BN254_BIG;

    int i, iterations;
    clock_t start;
    double elapsed;

    ECP P, G;
    ECP2 Q, W;
    FP12 g, w;
    FP4 cm;
    FP2 rz2;
    FP rz;

    BIG s, r, x, y;
    printf("\nTesting/Timing BN254 Pairings\n");

    ECP_generator(&G);
    BIG_rcopy(r, CURVE_Order);
    FP_rand(&rz,RNG);
    ECP_map2point(&P,&rz);
    ECP_cfp(&P);

    if (ECP_isinf(&P))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }
    ECP_mul(&P,r);
    if (!ECP_isinf(&P))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }

    BIG_randtrunc(s, r, 2 * CURVE_SECURITY_BN254, RNG);

    ECP_copy(&P, &G);
    PAIR_G1mul(&P, r);

    if (!ECP_isinf(&P))
    {
        printf("FAILURE - rG!=O\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        ECP_copy(&P, &G);
        PAIR_G1mul(&P, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("G1 mul              - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    ECP2_generator(&W);

    FP2_rand(&rz2,RNG);
    ECP2_map2point(&Q,&rz2);
    ECP2_cfp(&Q);

    if (ECP2_isinf(&Q))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }
    //printf("Q= "); ECP2_output(&Q); printf("\n");
    ECP2_mul(&Q,r);
    if (!ECP2_isinf(&Q))
    {
        printf("FAILURE - rQ!=O\n");
        return 0;
    }

    ECP2_copy(&Q, &W);
    ECP2_mul(&Q, r);

    if (!ECP2_isinf(&Q))
    {
        printf("FAILURE - rQ!=O\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        ECP2_copy(&Q, &W);
        PAIR_G2mul(&Q, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("G2 mul              - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    PAIR_ate(&w, &Q, &P);
    PAIR_fexp(&w);

//printf("pairing= ");FP12_output(&w); printf("\n");

    FP12_copy(&g, &w);

    PAIR_GTpow(&g, r);

    if (!FP12_isunity(&g))
    {
        printf("FAILURE - g^r!=1\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        FP12_copy(&g, &w);
        PAIR_GTpow(&g, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("GT pow              - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    FP12_copy(&g, &w);

    iterations = 0;
    start = clock();
    do {
        FP12_compow(&cm, &g, s, r);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("GT pow (compressed) - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    iterations = 0;
    start = clock();
    do {
        PAIR_ate(&w, &Q, &P);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("PAIRing ATE         - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    iterations = 0;
    start = clock();
    do {
        FP12_copy(&g, &w);
        PAIR_fexp(&g);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("PAIRing FEXP        - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    ECP_copy(&P, &G);
    ECP2_copy(&Q, &W);

    PAIR_G1mul(&P, s);
    PAIR_ate(&g, &Q, &P);
    PAIR_fexp(&g);

    ECP_copy(&P, &G);

    PAIR_G2mul(&Q, s);
    PAIR_ate(&w, &Q, &P);
    PAIR_fexp(&w);

	if (!PAIR_G1member(&P))
	{
        printf("FAILURE - P not in G1\n");
        return 0;
	}

	if (!PAIR_G2member(&Q))
	{
        printf("FAILURE - Q not in G2\n");
        return 0;
	}

	if (!PAIR_GTmember(&w))
	{
        printf("FAILURE - e(Q,P) not in GT\n");
        return 0;
	}

    if (!FP12_equals(&g, &w))
    {
        printf("FAILURE - e(sQ,p)!=e(Q,sP) \n");
        return 0;
    }

    ECP2_copy(&Q, &W);
    PAIR_ate(&g, &Q, &P);
    PAIR_fexp(&g);

    PAIR_GTpow(&g, s);

    if (!FP12_equals(&g, &w))
    {
        printf("FAILURE - e(sQ,p)!=e(Q,P)^s \n");
        return 0;
    }

    ECP N1;
    ECP2 N2;

    ECP_inf(&N1);
    ECP2_inf(&N2);

    PAIR_ate(&g, &N2, &P);
    PAIR_fexp(&g);

    if (!FP12_isunity(&g))
    {
        printf("FAILURE - e(O,P) !=1 \n");
        return 0;
    }

    PAIR_ate(&g, &Q, &N1);
    PAIR_fexp(&g);

    if (!FP12_isunity(&g))
    {
        printf("FAILURE - e(Q,O) !=1 \n");
        return 0;
    }
 
	

    return 0;
}

#if CHUNK==32 || CHUNK==64
int bls383(csprng *RNG)
{
    using namespace BLS12383;
    using namespace BLS12383_FP;
    using namespace BLS12383_BIG;

    int i, iterations;
    clock_t start;
    double elapsed;

    ECP P, G;
    ECP2 Q, W;
    FP12 g, w;
    FP4 cm;
    FP2 rz2;
    FP rz;

    BIG s, r, x, y;
    printf("\nTesting/Timing BLS12383 Pairings\n");

    ECP_generator(&G);
    BIG_rcopy(r, CURVE_Order);
    FP_rand(&rz,RNG);
    ECP_map2point(&P,&rz);
    ECP_cfp(&P);

    if (ECP_isinf(&P))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }
    ECP_mul(&P,r);
    if (!ECP_isinf(&P))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }

    BIG_randtrunc(s, r, 2 * CURVE_SECURITY_BLS12383, RNG);


    ECP_copy(&P, &G);
    PAIR_G1mul(&P, r);

    if (!ECP_isinf(&P))
    {
        printf("FAILURE - rG!=O\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        ECP_copy(&P, &G);
        PAIR_G1mul(&P, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("G1 mul              - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    ECP2_generator(&W);

    FP2_rand(&rz2,RNG);
    ECP2_map2point(&Q,&rz2);
    ECP2_cfp(&Q);

    if (ECP2_isinf(&Q))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }
    ECP2_mul(&Q,r);
    if (!ECP2_isinf(&Q))
    {
        printf("FAILURE - rQ!=O\n");
        return 0;
    }


    ECP2_copy(&Q, &W);
    ECP2_mul(&Q, r);

    if (!ECP2_isinf(&Q))
    {
        printf("FAILURE - rQ!=O\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        ECP2_copy(&Q, &W);
        PAIR_G2mul(&Q, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("G2 mul              - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    PAIR_ate(&w, &Q, &P);
    PAIR_fexp(&w);

    FP12_copy(&g, &w);
    PAIR_GTpow(&g, r);

    if (!FP12_isunity(&g))
    {
        printf("FAILURE - g^r!=1\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        FP12_copy(&g, &w);
        PAIR_GTpow(&g, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("GT pow              - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    FP12_copy(&g, &w);

    iterations = 0;
    start = clock();
    do {
        FP12_compow(&cm, &g, s, r);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("GT pow (compressed) - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    iterations = 0;
    start = clock();
    do {
        PAIR_ate(&w, &Q, &P);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("PAIRing ATE         - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    iterations = 0;
    start = clock();
    do {
        FP12_copy(&g, &w);
        PAIR_fexp(&g);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("PAIRing FEXP        - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    ECP_copy(&P, &G);
    ECP2_copy(&Q, &W);

    PAIR_G1mul(&P, s);
    PAIR_ate(&g, &Q, &P);
    PAIR_fexp(&g);

    ECP_copy(&P, &G);

    PAIR_G2mul(&Q, s);
    PAIR_ate(&w, &Q, &P);
    PAIR_fexp(&w);

	if (!PAIR_G1member(&P))
	{
        printf("FAILURE - P not in G1\n");
        return 0;
	}

	if (!PAIR_G2member(&Q))
	{
        printf("FAILURE - Q not in G2\n");
        return 0;
	}

	if (!PAIR_GTmember(&w))
	{
        printf("FAILURE - e(Q,P) not in GT\n");
        return 0;
	}

    if (!FP12_equals(&g, &w))
    {
        printf("FAILURE - e(sQ,p)!=e(Q,sP) \n");
        return 0;
    }

    ECP2_copy(&Q, &W);
    PAIR_ate(&g, &Q, &P);
    PAIR_fexp(&g);

    PAIR_GTpow(&g, s);

    if (!FP12_equals(&g, &w))
    {
        printf("FAILURE - e(sQ,p)!=e(Q,P)^s \n");
        return 0;
    }

    ECP N1;
    ECP2 N2;

    ECP_inf(&N1);
    ECP2_inf(&N2);

    PAIR_ate(&g, &N2, &P);
    PAIR_fexp(&g);

    if (!FP12_isunity(&g))
    {
        printf("FAILURE - e(O,P) !=1 \n");
        return 0;
    }

    PAIR_ate(&g, &Q, &N1);
    PAIR_fexp(&g);

    if (!FP12_isunity(&g))
    {
        printf("FAILURE - e(Q,O) !=1 \n");
        return 0;
    }

    return 0;
}

int bls24(csprng *RNG)
{
    using namespace BLS24479;
    using namespace BLS24479_FP;
    using namespace BLS24479_BIG;

    int i, iterations;
    clock_t start;
    double elapsed;

    ECP P, G;
    ECP4 Q, W;
    FP24 g, w;
    FP rz;
    FP4 rz4;

    FP8 cm;
    BIG a, b, s, r;

    printf("\nTesting/Timing BLS24479 Pairings\n");

    ECP_generator(&G);
    BIG_rcopy(r, CURVE_Order);
    FP_rand(&rz,RNG);
    ECP_map2point(&P,&rz);
    ECP_cfp(&P);

    if (ECP_isinf(&P))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }
    ECP_mul(&P,r);
    if (!ECP_isinf(&P))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }

    BIG_randtrunc(s, r, 2 * CURVE_SECURITY_BLS24479, RNG);

    ECP_copy(&P, &G);
    PAIR_G1mul(&P, r);

    if (!ECP_isinf(&P))
    {
        printf("FAILURE - rG!=O\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        ECP_copy(&P, &G);
        PAIR_G1mul(&P, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("G1 mul              - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    ECP4_generator(&W);

    FP4_rand(&rz4,RNG);
    ECP4_map2point(&Q,&rz4);
    ECP4_cfp(&Q);

    if (ECP4_isinf(&Q))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }
    ECP4_mul(&Q,r);
    if (!ECP4_isinf(&Q))
    {
        printf("FAILURE - rQ!=O\n");
        return 0;
    }
    ECP4_copy(&Q, &W);
    ECP4_mul(&Q, r);

    if (!ECP4_isinf(&Q))
    {
        printf("FAILURE - rQ!=O\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        ECP4_copy(&Q, &W);
        PAIR_G2mul(&Q, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("G2 mul              - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    PAIR_ate(&w, &Q, &P);
    PAIR_fexp(&w);

    FP24_copy(&g, &w);

    PAIR_GTpow(&g, r);

    if (!FP24_isunity(&g))
    {
        printf("FAILURE - g^r!=1\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        FP24_copy(&g, &w);
        PAIR_GTpow(&g, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("GT pow              - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);
/*
    FP24_copy(&g, &w);

    iterations = 0;
    start = clock();
    do {
        FP24_compow(&cm, &g, s, r);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("GT pow (compressed) - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);
*/
    iterations = 0;
    start = clock();
    do {
        PAIR_ate(&w, &Q, &P);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("PAIRing ATE         - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    iterations = 0;
    start = clock();
    do {
        FP24_copy(&g, &w);
        PAIR_fexp(&g);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("PAIRing FEXP        - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    ECP_copy(&P, &G);
    ECP4_copy(&Q, &W);

    PAIR_G1mul(&P, s);
    PAIR_ate(&g, &Q, &P);
    PAIR_fexp(&g);

    ECP_copy(&P, &G);

    PAIR_G2mul(&Q, s);
    PAIR_ate(&w, &Q, &P);
    PAIR_fexp(&w);

	if (!PAIR_G1member(&P))
	{
        printf("FAILURE - P not in G1\n");
        return 0;
	}

	if (!PAIR_G2member(&Q))
	{
        printf("FAILURE - Q not in G2\n");
        return 0;
	}

	if (!PAIR_GTmember(&w))
	{
        printf("FAILURE - e(Q,P) not in GT\n");
        return 0;
	}

    if (!FP24_equals(&g, &w))
    {
        printf("FAILURE - e(sQ,p)!=e(Q,sP) \n");
        return 0;
    }

    ECP4_copy(&Q, &W);
    PAIR_ate(&g, &Q, &P);
    PAIR_fexp(&g);

    PAIR_GTpow(&g, s);

    if (!FP24_equals(&g, &w))
    {
        printf("FAILURE - e(sQ,p)!=e(Q,P)^s \n");
        return 0;
    }

    ECP N1;
    ECP4 N2;

    ECP_inf(&N1);
    ECP4_inf(&N2);

    PAIR_ate(&g, &N2, &P);
    PAIR_fexp(&g);

    if (!FP24_isunity(&g))
    {
        printf("FAILURE - e(O,P) !=1 \n");
        return 0;
    }

    PAIR_ate(&g, &Q, &N1);
    PAIR_fexp(&g);

    if (!FP24_isunity(&g))
    {
        printf("FAILURE - e(Q,O) !=1 \n");
        return 0;
    }
    /*
        PAIR_ate(&g,&N2,&N1);
        PAIR_fexp(&g);

        if (!FP24_isunity(&g))
        {
            printf("FAILURE - e(O,O) !=1 \n");
            return 0;
        }
    */
    return 0;
}

int bls48(csprng *RNG)
{
    using namespace BLS48556;
    using namespace BLS48556_FP;
    using namespace BLS48556_BIG;

    int i, iterations;
    clock_t start;
    double elapsed;

    ECP P, G;
    ECP8 Q, W;
    FP48 g, w;
    FP rz;
    FP8 rz8;

    FP16 cm;
    BIG a, b, s, r;

    printf("\nTesting/Timing BLS48556 Pairings\n");

    ECP_generator(&G);
    BIG_rcopy(r, CURVE_Order);
    FP_rand(&rz,RNG);
    ECP_map2point(&P,&rz);
    ECP_cfp(&P);

    if (ECP_isinf(&P))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }
    ECP_mul(&P,r);
    if (!ECP_isinf(&P))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }

    BIG_randtrunc(s, r, 2 * CURVE_SECURITY_BLS48556, RNG);

    ECP_copy(&P, &G);
    PAIR_G1mul(&P, r);

    if (!ECP_isinf(&P))
    {
        printf("FAILURE - rG!=O\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        ECP_copy(&P, &G);
        PAIR_G1mul(&P, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("G1 mul              - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);


    ECP8_generator(&W);

    FP8_rand(&rz8,RNG);
    ECP8_map2point(&Q,&rz8);
    ECP8_cfp(&Q);

    if (ECP8_isinf(&Q))
    {
        printf("HASHING FAILURE - P=O\n");
        return 0;
    }

    ECP8_mul(&Q,r);
    if (!ECP8_isinf(&Q))
    {
        printf("FAILURE - rQ!=O\n");
        return 0;
    }
    ECP8_copy(&Q, &W);

    //ECP8_mul(&Q,r);
    PAIR_G2mul(&Q, r);

    if (!ECP8_isinf(&Q))
    {
        printf("FAILURE - rQ!=O\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        ECP8_copy(&Q, &W);
        PAIR_G2mul(&Q, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("G2 mul              - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);


    PAIR_ate(&w, &Q, &P);
    PAIR_fexp(&w);

    FP48_copy(&g, &w);

    PAIR_GTpow(&g, r);

    if (!FP48_isunity(&g))
    {
        printf("FAILURE - g^r!=1\n");
        return 0;
    }

    iterations = 0;
    start = clock();
    do {
        FP48_copy(&g, &w);
        PAIR_GTpow(&g, s);

        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("GT pow              - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);
/*
    FP48_copy(&g, &w);

    iterations = 0;
    start = clock();
    do {
        FP48_compow(&cm, &g, s, r);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("GT pow (compressed) - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);
*/
    iterations = 0;
    start = clock();
    do {
        PAIR_ate(&w, &Q, &P);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("PAIRing ATE         - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    iterations = 0;
    start = clock();
    do {
        FP48_copy(&g, &w);
        PAIR_fexp(&g);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("PAIRing FEXP        - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    ECP_copy(&P, &G);
    ECP8_copy(&Q, &W);

    PAIR_G1mul(&P, s);
    PAIR_ate(&g, &Q, &P);
    PAIR_fexp(&g);

    ECP_copy(&P, &G);

    PAIR_G2mul(&Q, s);
    //ECP8_mul(&Q,s);
    PAIR_ate(&w, &Q, &P);
    PAIR_fexp(&w);

	if (!PAIR_G1member(&P))
	{
        printf("FAILURE - P not in G1\n");
        return 0;
	}

	if (!PAIR_G2member(&Q))
	{
        printf("FAILURE - Q not in G2\n");
        return 0;
	}

	if (!PAIR_GTmember(&w))
	{
        printf("FAILURE - e(Q,P) not in GT\n");
        return 0;
	}

    if (!FP48_equals(&g, &w))
    {
        printf("FAILURE - e(sQ,p)!=e(Q,sP) \n");
        return 0;
    }

    ECP8_copy(&Q, &W);
    PAIR_ate(&g, &Q, &P);
    PAIR_fexp(&g);

    PAIR_GTpow(&g, s);

    if (!FP48_equals(&g, &w))
    {
        printf("FAILURE - e(sQ,p)!=e(Q,P)^s \n");
        return 0;
    }

    ECP N1;
    ECP8 N2;

    ECP_inf(&N1);
    ECP8_inf(&N2);

    PAIR_ate(&g, &N2, &P);
    PAIR_fexp(&g);

    if (!FP48_isunity(&g))
    {
        printf("FAILURE - e(O,P) !=1 \n");
        return 0;
    }

    PAIR_ate(&g, &Q, &N1);
    PAIR_fexp(&g);

    if (!FP48_isunity(&g))
    {
        printf("FAILURE - e(Q,O) !=1 \n");
        return 0;
    }
    /*
        PAIR_ate(&g,&N2,&N1);
        PAIR_fexp(&g);

        if (!FP48_isunity(&g))
        {
            printf("FAILURE - e(O,O) !=1 \n");
            return 0;
        }
    */
    return 0;
}
#endif

int rsa2048(csprng *RNG)
{
    using namespace RSA2048;

    rsa_public_key pub;
    rsa_private_key priv;

    int i, iterations;
    clock_t start;
    double elapsed;

    char m[RFS_RSA2048], d[RFS_RSA2048], c[RFS_RSA2048];
    octet M = {0, sizeof(m), m};
    octet D = {0, sizeof(d), d};
    octet C = {0, sizeof(c), c};

    printf("\nTesting/Timing 2048-bit RSA\n");

    printf("Generating 2048-bit RSA public/private key pair\n");

    iterations = 0;
    start = clock();
    do {
        RSA_KEY_PAIR(RNG, 65537, &priv, &pub, NULL, NULL);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < 1);
    elapsed = 1000.0 * elapsed / iterations;
    printf("RSA gen - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    M.len = RFS_RSA2048;
    for (i = 0; i < RFS_RSA2048; i++) M.val[i] = i % 128;

    iterations = 0;
    start = clock();
    do {
        RSA_ENCRYPT(&pub, &M, &C);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("RSA enc - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    iterations = 0;
    start = clock();
    do {
        RSA_DECRYPT(&priv, &C, &D);
        iterations++;
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < MIN_TIME || iterations < MIN_ITERS);
    elapsed = 1000.0 * elapsed / iterations;
    printf("RSA dec - %8d iterations  ", iterations);
    printf(" %8.2lf ms per iteration\n", elapsed);

    for (i = 0; i < RFS_RSA2048; i++)
    {
        if (M.val[i] != D.val[i])
        {
            printf("FAILURE - RSA decryption\n");
            return 0;
        }
    }

    printf("All tests pass\n");

    return 0;
}

int main()
{
    csprng RNG;
    int i;
    char pr[10];
    unsigned long ran;

    time((time_t *)&ran);
    pr[0] = ran;
    pr[1] = ran >> 8;
    pr[2] = ran >> 16;
    pr[3] = ran >> 24;
    for (i = 4; i < 10; i++) pr[i] = i; /*****4****/
    RAND_seed(&RNG, 10, pr);

    ed25519(&RNG);
#if CHUNK==32 || CHUNK==64
    nist256(&RNG);
    goldilocks(&RNG);
#endif
    bn254(&RNG);
#if CHUNK==32 || CHUNK==64
    bls383(&RNG);
    bls24(&RNG);
    bls48(&RNG);
#endif
    rsa2048(&RNG);

}
