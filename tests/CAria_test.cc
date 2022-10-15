// Copyright 2021~2022 `anothel` All rights reserved

#include "include/CAria.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(CAria, excute_test)
{
    Awesome_mix_vol_1::CAria::CHECK_ENDIAN();
    Awesome_mix_vol_1::CAria aria;

    // Make Master Key(mk)
    Byte rk[16 * 17] = {0}, mk[32] = {0}, c[16] = {0};
    for (int i = 0; i < 16; i++)
        mk[i] = i * 0x11;
    for (int i = 16; i < 24; i++)
        mk[i] = (i - 16) * 0x11;

    Byte p[16] = {0x11, 0x11, 0x11, 0x11, 0xaa, 0xaa, 0xaa, 0xaa,
                  0x11, 0x11, 0x11, 0x11, 0xbb, 0xbb, 0xbb, 0xbb};
    aria.Crypt(p, aria.EncKeySetup(mk, rk, 192), rk, c);
    printf("BEGIN testing basic encryption...\n");
    printf(
        "Testing whether the encryption would come out correctly,"
        " for 14-round ARIA.\n");

    printf("key      : ");
    aria.printBlockOfLength(mk, 24);
    printf("\n");

    printf("plaintext: ");
    aria.printBlock(p);
    printf("\n");

    printf("result is: ");
    aria.printBlock(c);
    printf("\n");

    printf("should be: ");
    const Byte cryptResult[] = {0x8d, 0x14, 0x70, 0x62, 0x5f, 0x59, 0xeb, 0xac,
                                0xb0, 0xe5, 0x5b, 0x53, 0x4b, 0x3e, 0x46, 0x2b};
    aria.printBlock(const_cast<Byte *>(cryptResult));
    printf("\n");

    for (int i = 0; i < 16; i++)
    {
        if (c[i] != cryptResult[i])
        {
            printf("The result is incorrect!\n");
            exit(1);
        }
    }
    printf("END   testing basic encryption.\n\n");

    for (int i = 0; i < 32; i++)
        mk[i] = 0;
    for (int i = 0; i < 16; i++)
        p[i] = 0;

    printf("BEGIN testing the roundtrip...\n");
    printf(
        "For key size of 192 bits, starting with the zero plaintext and the zero "
        "key, let's see if we may recover the plaintext by decrypting the "
        "encrypted ciphertext.\n");
    aria.EncKeySetup(mk, rk, 192);
    printf("plaintext : ");
    aria.printBlock(p);
    printf("\n");

    aria.Crypt(p, 14, rk, c);
    printf("ciphertext: ");
    aria.printBlock(c);
    printf("\n");

    aria.DecKeySetup(mk, rk, 192);
    aria.Crypt(c, 14, rk, p);
    printf("decrypted : ");
    aria.printBlock(p);
    printf("\n");

    for (int i = 0; i < 16; i++)
    {
        if (p[i] != 0)
        {
            printf("The result is incorrect!\n");
            exit(1);
        }
    }
    printf("END   testing the roundtrip.\n");
}
