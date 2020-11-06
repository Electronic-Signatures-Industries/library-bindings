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

/* Boneh-Lynn-Shacham signature 192-bit API */

/* Loosely (for now) following https://datatracker.ietf.org/doc/html/draft-irtf-cfrg-bls-signature-00 */

// Minimal-pubkey-size variant

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bls192_ZZZ.h"

using namespace XXX;
using namespace YYY;

#define CEIL(a,b) (((a)-1)/(b)+1)

/* output u[i] \in F_p */
/* https://datatracker.ietf.org/doc/draft-irtf-cfrg-hash-to-curve/ */
static void hash_to_field(int hash,int hlen,FP4 *u,octet *DST,octet *M, int ctr)
{
    int i,j,k,L;
    BIG q,a[4];
    FP2 c,d;
    DBIG dx;
    char okm[1024],fd[128];
    octet OKM = {0,sizeof(okm),okm};

    BIG_rcopy(q, Modulus);
    L=CEIL(BIG_nbits(q)+CURVE_SECURITY_ZZZ,8);

    XMD_Expand(hash,hlen,&OKM,L*ctr*4,DST,M);
    for (i=0;i<ctr;i++)
    {
        for (k=0;k<4;k++)
        {
            for (j=0;j<L;j++)
                fd[j]=OKM.val[(4*i+k)*L+j];
        
            BIG_dfromBytesLen(dx,fd,L);
            BIG_dmod(a[k],dx,q);
        }
        FP2_from_BIGs(&c,a[0],a[1]);
        FP2_from_BIGs(&d,a[2],a[3]);
        FP4_from_FP2s(&u[i],&c,&d);
    }
}

/* hash a message to an ECP4 point, using SHA2, random oracle method */
static void BLS_HASH_TO_POINT(ECP4 *P, octet *M)
{
    FP4 u[2];
    ECP4 P1;
    char dst[50];
    octet DST = {0,sizeof(dst),dst};

    OCT_jstring(&DST,(char *)"BLS_SIG_ZZZG2_XMD:SHA384-SVDW-RO-_NUL_");
    hash_to_field(MC_SHA2,HASH_TYPE_ZZZ,u,&DST,M,2);

    ECP4_map2point(P,&u[0]);
    ECP4_map2point(&P1,&u[1]);

    ECP4_add(P,&P1);
    ECP4_cfp(P);
    ECP4_affine(P);
}

int ZZZ::BLS_INIT()
{
    return BLS_OK;
}

/* generate key pair, private key S, public key W */
int ZZZ::BLS_KEY_PAIR_GENERATE(octet *IKM, octet* S, octet *W)
{
    int L;
    BIG r,s;
    DBIG dx;
    ECP G;
    char salt[20],prk[HASH_TYPE_ZZZ],okm[128],aikm[65],len[2];
    octet SALT = {0,sizeof(salt),salt};
    octet PRK = {0,sizeof(prk),prk};
    octet OKM = {0,sizeof(okm),okm};
    octet AIKM = {0,sizeof(aikm),aikm};
    octet LEN = {0,sizeof(len),len};

    OCT_copy(&AIKM,IKM);
    OCT_jbyte(&AIKM,0,1);

    BIG_rcopy(r, CURVE_Order);
    L=CEIL(3*CEIL(BIG_nbits(r),8),2);
    OCT_jint(&LEN,L,2);

    if (!ECP_generator(&G)) return BLS_FAIL;

    OCT_jstring(&SALT,(char *)"BLS-SIG-KEYGEN-SALT-");
    HKDF_Extract(MC_SHA2,HASH_TYPE_ZZZ,&PRK,&SALT,&AIKM);
    HKDF_Expand(MC_SHA2,HASH_TYPE_ZZZ,&OKM,L,&PRK,&LEN);

    BIG_dfromBytesLen(dx,OKM.val,L);
    BIG_dmod(s,dx,r);
    BIG_toBytes(S->val, s);
    S->len = MODBYTES_XXX;

// SkToPk

    PAIR_G1mul(&G, s);
    ECP_toOctet(W, &G, true);
    return BLS_OK;
}

/* Sign message M using private key S to produce signature SIG */

int ZZZ::BLS_CORE_SIGN(octet *SIG, octet *M, octet *S)
{
    BIG s;
    ECP4 D;
    BLS_HASH_TO_POINT(&D, M);
    BIG_fromBytes(s, S->val);
    PAIR_G2mul(&D, s);

    ECP4_toOctet(SIG, &D, true); /* compress output */
    return BLS_OK;
}

/* Verify signature given message m, the signature SIG, and the public key W */
int ZZZ::BLS_CORE_VERIFY(octet *SIG, octet *M, octet *W)
{
    FP24 v;
    ECP G, PK;
    ECP4 D, HM;

    BLS_HASH_TO_POINT(&HM, M);

    ECP4_fromOctet(&D, SIG);
	if (!PAIR_G2member(&D)) return BLS_FAIL;
    ECP4_neg(&D);

    ECP_fromOctet(&PK, W);
    if (!ECP_generator(&G)) return BLS_FAIL;
    PAIR_double_ate(&v, &D, &G, &HM, &PK);

    PAIR_fexp(&v);
    if (FP24_isunity(&v)) return BLS_OK;
    return BLS_FAIL;
}

