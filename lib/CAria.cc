// Copyright 2021~2022 `anothel` All rights reserved

/*
 * A 32-bit implementation for ARIA
 *
 * follows the specifications given in
 * the ARIA specification at
 * http://www.nsri.re.kr/ARIA/
 *
 * Note:
 *    - Main body optimized for speed for 32 bit platforms
 *       * Utilizes 32-bit optimization techniques presented in ICISC 2003
 *       * Only four 32-bit tables are used
 *
 *    - Implemented some ideas for optimization from the creators of ARIA,
 *         and adopted some ideas from works submitted to ARIA implementation
 * contest on Aug. 2004.
 *
 *    - Handles endian problem pretty well.
 *       * For optimization, for little endian architecture key setup functions
 * return endian-reversed round keys; Crypt() function handles this correctly.
 *
 * 17, January 2005
 * Aaram Yun
 * National Security Research Institute, KOREA
 *
 * Substantial portion of the code originally written by Jin Hong.
 *
 */

/* 사용 플랫폼의 endian 특성에 따라 _LITTLE_ENDIAN_ 혹은
 * _BIG_ENDIAN_ 둘 중 하나를 정의해야 컴파일 됩니다.
 * Windows+Intel 플랫폼의 경우에는 _LITTLE_ENDIAN_이고,
 * 그 외에는 많은 경우 _BIG_ENDIAN_입니다.  잘 모르겠으면
 * 아무 쪽이나 선택해서 컴파일 후 실행하십시오.  ARIA_test()
 * 함수에서 ENDIAN 확인을 하기 때문에 올바른 선택이었는지를
 * 점검할 수 있습니다. */

/*********************************************************/

#include "include/CAria.hpp"

#include <fmt/core.h>

#include <cstdio>
#include <cstdlib>

namespace Awesome_mix_vol_1 {

void CAria::printBlockOfLength(Byte *b, int len) {
  int i;

  for (i = 0; i < len; i++, b++) {
    printf("%02x", *b);
    if (i % 4 == 3 && i < len - 1) printf(" ");
  }
}

void CAria::printBlock(Byte *b) { printBlockOfLength(b, 16); }

/* 암호화 함수.
 * const Byte *i: 입력
 * int Nr: 라운드 수
 * const Byte *rk: 라운드 키들
 * Byte *o: 출력
 */
void CAria::Crypt(const Byte *i, int Nr, const Byte *rk, Byte *o) {
  Word t0, t1, t2, t3;

  WordLoad(WO(const_cast<Byte *>(i), 0), t0);
  WordLoad(WO(const_cast<Byte *>(i), 1), t1);
  WordLoad(WO(const_cast<Byte *>(i), 2), t2);
  WordLoad(WO(const_cast<Byte *>(i), 3), t3);

  if (Nr > 12) {
    KXL FO KXL FE
  }
  if (Nr > 14) {
    KXL FO KXL FE
  }
  KXL FO KXL FE KXL FO KXL FE KXL FO KXL FE KXL FO KXL FE KXL FO KXL FE KXL FO
      KXL

  /* 최종 라운드는 특별함 */
#ifdef _LITTLE_ENDIAN_
          o[0] = (Byte)(X1[BRF(t0, 24)]) ^ rk[3];
  o[1] = (Byte)(X2[BRF(t0, 16)] >> 8) ^ rk[2];
  o[2] = (Byte)(S1[BRF(t0, 8)]) ^ rk[1];
  o[3] = (Byte)(S2[BRF(t0, 0)]) ^ rk[0];
  o[4] = (Byte)(X1[BRF(t1, 24)]) ^ rk[7];
  o[5] = (Byte)(X2[BRF(t1, 16)] >> 8) ^ rk[6];
  o[6] = (Byte)(S1[BRF(t1, 8)]) ^ rk[5];
  o[7] = (Byte)(S2[BRF(t1, 0)]) ^ rk[4];
  o[8] = (Byte)(X1[BRF(t2, 24)]) ^ rk[11];
  o[9] = (Byte)(X2[BRF(t2, 16)] >> 8) ^ rk[10];
  o[10] = (Byte)(S1[BRF(t2, 8)]) ^ rk[9];
  o[11] = (Byte)(S2[BRF(t2, 0)]) ^ rk[8];
  o[12] = (Byte)(X1[BRF(t3, 24)]) ^ rk[15];
  o[13] = (Byte)(X2[BRF(t3, 16)] >> 8) ^ rk[14];
  o[14] = (Byte)(S1[BRF(t3, 8)]) ^ rk[13];
  o[15] = (Byte)(S2[BRF(t3, 0)]) ^ rk[12];
#else
          o[0] = (Byte)(X1[BRF(t0, 24)]);
  o[1] = (Byte)(X2[BRF(t0, 16)] >> 8);
  o[2] = (Byte)(S1[BRF(t0, 8)]);
  o[3] = (Byte)(S2[BRF(t0, 0)]);
  o[4] = (Byte)(X1[BRF(t1, 24)]);
  o[5] = (Byte)(X2[BRF(t1, 16)] >> 8);
  o[6] = (Byte)(S1[BRF(t1, 8)]);
  o[7] = (Byte)(S2[BRF(t1, 0)]);
  o[8] = (Byte)(X1[BRF(t2, 24)]);
  o[9] = (Byte)(X2[BRF(t2, 16)] >> 8);
  o[10] = (Byte)(S1[BRF(t2, 8)]);
  o[11] = (Byte)(S2[BRF(t2, 0)]);
  o[12] = (Byte)(X1[BRF(t3, 24)]);
  o[13] = (Byte)(X2[BRF(t3, 16)] >> 8);
  o[14] = (Byte)(S1[BRF(t3, 8)]);
  o[15] = (Byte)(S2[BRF(t3, 0)]);
  WO(o, 0) ^= WO(const_cast<Byte *>(rk), 0);
  WO(o, 1) ^= WO(const_cast<Byte *>(rk), 1);
  WO(o, 2) ^= WO(const_cast<Byte *>(rk), 2);
  WO(o, 3) ^= WO(const_cast<Byte *>(rk), 3);
#endif
}

/* 암호화 라운드 키 생성
 * const Byte *mk: 마스터 키
 * Byte *rk: 라운드 키
 * int keyBits: 마스터 키의 길이
 * return: 라운드 수
 */
int CAria::EncKeySetup(const Byte *mk, Byte *rk, int keyBits) {
  Word t0, t1, t2, t3;
  Word w0[4], w1[4], w2[4], w3[4];
  int q, r;

  WordLoad(WO(const_cast<Byte *>(mk), 0), w0[0]);
  WordLoad(WO(const_cast<Byte *>(mk), 1), w0[1]);
  WordLoad(WO(const_cast<Byte *>(mk), 2), w0[2]);
  WordLoad(WO(const_cast<Byte *>(mk), 3), w0[3]);

  q = (keyBits - 128) / 64;
  t0 = w0[0] ^ KRK[q][0];
  t1 = w0[1] ^ KRK[q][1];
  t2 = w0[2] ^ KRK[q][2];
  t3 = w0[3] ^ KRK[q][3];
  FO;
  if (keyBits > 128) {
    WordLoad(WO(const_cast<Byte *>(mk), 4), w1[0]);
    WordLoad(WO(const_cast<Byte *>(mk), 5), w1[1]);
    if (keyBits > 192) {
      WordLoad(WO(const_cast<Byte *>(mk), 6), w1[2]);
      WordLoad(WO(const_cast<Byte *>(mk), 7), w1[3]);
    } else {
      w1[2] = w1[3] = 0;
    }
  } else {
    w1[0] = w1[1] = w1[2] = w1[3] = 0;
  }
  w1[0] ^= t0;
  w1[1] ^= t1;
  w1[2] ^= t2;
  w1[3] ^= t3;
  t0 = w1[0];
  t1 = w1[1];
  t2 = w1[2];
  t3 = w1[3];

  q = (q == 2) ? 0 : (q + 1);
  t0 ^= KRK[q][0];
  t1 ^= KRK[q][1];
  t2 ^= KRK[q][2];
  t3 ^= KRK[q][3];
  FE;
  t0 ^= w0[0];
  t1 ^= w0[1];
  t2 ^= w0[2];
  t3 ^= w0[3];
  w2[0] = t0;
  w2[1] = t1;
  w2[2] = t2;
  w2[3] = t3;

  q = (q == 2) ? 0 : (q + 1);
  t0 ^= KRK[q][0];
  t1 ^= KRK[q][1];
  t2 ^= KRK[q][2];
  t3 ^= KRK[q][3];
  FO;
  w3[0] = t0 ^ w1[0];
  w3[1] = t1 ^ w1[1];
  w3[2] = t2 ^ w1[2];
  w3[3] = t3 ^ w1[3];

  GSRK(w0, w1, 19);
  GSRK(w1, w2, 19);
  GSRK(w2, w3, 19);
  GSRK(w3, w0, 19);
  GSRK(w0, w1, 31);
  GSRK(w1, w2, 31);
  GSRK(w2, w3, 31);
  GSRK(w3, w0, 31);
  GSRK(w0, w1, 67);
  GSRK(w1, w2, 67);
  GSRK(w2, w3, 67);
  GSRK(w3, w0, 67);
  GSRK(w0, w1, 97);
  if (keyBits > 128) {
    GSRK(w1, w2, 97);
    GSRK(w2, w3, 97);
  }
  if (keyBits > 192) {
    GSRK(w3, w0, 97);
    GSRK(w0, w1, 109);
  }

  return (keyBits + 256) / 32;
}

/* 복호화 라운드 키 생성
 * const Byte *mk: 마스터 키
 * Byte *rk: 라운드 키
 * int keyBits: 마스터 키의 길이
 */
int CAria::DecKeySetup(const Byte *mk, Byte *rk, int keyBits) {
  int rValue = EncKeySetup(mk, rk, keyBits);
  Word *a = reinterpret_cast<Word *>((rk));
  Word *z = a + rValue * 4;

  Word t0, t1, t2, t3;
  Word s0, s1, s2, s3;

  t0 = a[0];
  t1 = a[1];
  t2 = a[2];
  t3 = a[3];

  a[0] = z[0];
  a[1] = z[1];
  a[2] = z[2];
  a[3] = z[3];

  z[0] = t0;
  z[1] = t1;
  z[2] = t2;
  z[3] = t3;

  a += 4;
  z -= 4;

  for (; a < z; a += 4, z -= 4) {
    WordM1(a[0], t0);
    WordM1(a[1], t1);
    WordM1(a[2], t2);
    WordM1(a[3], t3);
    MM(t0, t1, t2, t3) P(t0, t1, t2, t3) MM(t0, t1, t2, t3) s0 = t0;
    s1 = t1;
    s2 = t2;
    s3 = t3;
    WordM1(z[0], t0);
    WordM1(z[1], t1);
    WordM1(z[2], t2);
    WordM1(z[3], t3);
    MM(t0, t1, t2, t3) P(t0, t1, t2, t3) MM(t0, t1, t2, t3) a[0] = t0;
    a[1] = t1;
    a[2] = t2;
    a[3] = t3;
    z[0] = s0;
    z[1] = s1;
    z[2] = s2;
    z[3] = s3;
  }

  WordM1(a[0], t0);
  WordM1(a[1], t1);
  WordM1(a[2], t2);
  WordM1(a[3], t3);

  MM(t0, t1, t2, t3) P(t0, t1, t2, t3) MM(t0, t1, t2, t3) z[0] = t0;

  z[1] = t1;
  z[2] = t2;
  z[3] = t3;

  return rValue;
}

void CAria::CHECK_ENDIAN() {
  const Word NUMBER = 0x00000042;
  Byte *b = reinterpret_cast<Byte *>(const_cast<Word *>(&NUMBER));

  printf("BEGIN testing endianness...\n");
  printf(
      "Since you are running this, it means that you have defined \neither "
      "_LITTLE_ENDIAN_ or _BIG_ENDIAN_. Let's see if you were correct.\n");

  if (b[0] == 0x42) {
    printf("We are on _LITTLE_ENDIAN_ platform.\n");
#ifdef _BIG_ENDIAN_
    printf("WARNING: _BIG_ENDIAN_ defined on _LITTLE_ENDIAN_ platform.\n");
    printf(
        "         You should define _LITTLE_ENDIAN_ instead of "
        "_BIG_ENDIAN_.\n");
    exit(1);
#else
    printf("Okay.  You were correct.\n");
#endif
  } else {
    printf("We are on _BIG_ENDIAN_ platform.\n");
#ifdef _LITTLE_ENDIAN_
    printf("WARNING: _LITTLE_ENDIAN_ defined on _BIG_ENDIAN_ platform.\n");
    printf(
        "         You should define _BIG_ENDIAN_ instead of "
        "_LITTLE_ENDIAN_.\n");
    exit(1);
#else
    printf("Okay.  You were correct.\n");
#endif
  }

  printf("END   testing endianness.\n\n");
  return;
}

}  // namespace Awesome_mix_vol_1
