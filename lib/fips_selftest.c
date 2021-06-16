//
// fips_selftest.c - Algorithm selftests for FIPS 140 compliance
//
// Copyright (c) Microsoft Corporation. Licensed under the MIT license.
//

#include "precomp.h"

#if SYMCRYPT_DO_FIPS_SELFTESTS

SYMCRYPT_FIPS_SELFTEST g_SymCryptFipsSelftestsPerformed = SYMCRYPT_SELFTEST_NONE;

#endif // SYMCRYPT_DO_FIPS_SELFTESTS

//
// Convenience structs for selftest data
//

typedef struct _SYMCRYPT_SELFTEST_DLKEY_2048
{
    BYTE public[256];
    BYTE private[32];
} SYMCRYPT_SELFTEST_DLKEY_2048;

typedef struct _SYMCRYPT_SELFTEST_DLGROUP_2048
{
    BYTE primeP[256];
    BYTE primeQ[32];
    BYTE generator[256];
    BYTE seed[32];
    UINT32 counter;
} SYMCRYPT_SELFTEST_DLGROUP_2048;

typedef struct _SYMCRYPT_SELFTEST_ECKEY_P256
{
    BYTE Qx[32];
    BYTE Qy[32];
    BYTE d[32];
} SYMCRYPT_SELFTEST_ECKEY_P256;

typedef struct _SYMCRYPT_SELFTEST_RSAKEY_2048
{
    UINT64 publicExp;
    BYTE modulus[256];
    BYTE prime1[128];
    BYTE prime2[128];
} SYMCRYPT_SELFTEST_RSAKEY_2048;

//
// DL groups and keys for DH secret agreement selftest
//

// Generator for the DL group used with the two following DH keys
const BYTE dhGenerator = 2;

// Keys generated from Oakley group 14 (IKE group mod p2048 from RFC 3526)
// aka SymCryptDlgroupDhSafePrimeParamsModp2048
const SYMCRYPT_SELFTEST_DLKEY_2048 dhKey1 =
{
    // Public key
    {
        0x7d, 0xca, 0xed, 0x67, 0x30, 0x4a, 0x28, 0xcb,
        0x6b, 0x18, 0x21, 0xb9, 0xdc, 0x82, 0xf5, 0xef,
        0xfe, 0x77, 0x35, 0xf2, 0xed, 0x91, 0x95, 0x9e,
        0x1f, 0xb6, 0xf9, 0x60, 0x84, 0x4f, 0x9f, 0xba,
        0x20, 0xc3, 0x1a, 0x66, 0xa2, 0x34, 0x42, 0x19,
        0x82, 0x52, 0xea, 0x53, 0x4f, 0x24, 0x93, 0x24,
        0xef, 0x4d, 0xba, 0x65, 0x49, 0x7f, 0x8d, 0x56,
        0x1c, 0x2f, 0xa6, 0x24, 0x26, 0x8c, 0xca, 0xad,
        0x28, 0x9d, 0x50, 0x2a, 0x41, 0xa0, 0x64, 0xfa,
        0xe6, 0x55, 0x43, 0xed, 0xdd, 0x6c, 0x3e, 0x45,
        0xad, 0xa4, 0x07, 0x8d, 0x71, 0x76, 0xc0, 0x99,
        0x85, 0x72, 0x33, 0x04, 0xdc, 0xee, 0x4e, 0xac,
        0x39, 0x8e, 0x49, 0x45, 0xbb, 0x50, 0xcb, 0x3e,
        0x03, 0xbc, 0x23, 0x6f, 0x63, 0x6f, 0xef, 0x9f,
        0xd8, 0xe4, 0x7f, 0xe7, 0x0f, 0x7d, 0x34, 0x0f,
        0x6a, 0xc3, 0x45, 0x3a, 0x5b, 0xa7, 0x07, 0x23,
        0xd5, 0x32, 0x28, 0xe4, 0x69, 0xa4, 0xd2, 0xdd,
        0x4a, 0x9e, 0x0f, 0xfa, 0x38, 0xeb, 0x8d, 0xbe,
        0xa9, 0x31, 0x39, 0x70, 0xdc, 0x1d, 0xf6, 0x0b,
        0x51, 0x0c, 0x6c, 0x27, 0x6c, 0x4a, 0x52, 0x6b,
        0x84, 0xc4, 0x57, 0xb8, 0x47, 0x0c, 0x4c, 0x80,
        0xb0, 0x2b, 0x2e, 0x83, 0x78, 0x83, 0x55, 0xe6,
        0x12, 0x94, 0x79, 0x9b, 0x13, 0x7d, 0xd1, 0x93,
        0x6a, 0xec, 0x76, 0x73, 0xba, 0x92, 0x44, 0xab,
        0x2b, 0xec, 0xc2, 0xbc, 0x77, 0xce, 0x20, 0xa3,
        0x21, 0x33, 0x06, 0x80, 0xf6, 0x1b, 0x2b, 0xe0,
        0x90, 0x23, 0xde, 0x74, 0x57, 0x04, 0xcc, 0x27,
        0xed, 0x4e, 0x3e, 0x7f, 0x0d, 0x7b, 0x48, 0xfa,
        0x45, 0x2f, 0x84, 0x67, 0xe8, 0x1c, 0x89, 0xaa,
        0xbd, 0x85, 0x53, 0x7f, 0xe6, 0xdf, 0x27, 0x75,
        0x92, 0x84, 0x0b, 0x6f, 0x9c, 0xfd, 0x95, 0x0d,
        0xb5, 0xdb, 0x17, 0x38, 0xc9, 0x78, 0xde, 0xa5
    },

    // Private key
    {
        0x17, 0xcd, 0x0e, 0x82, 0xa3, 0x2f, 0x0e, 0x38,
        0x4c, 0xec, 0x69, 0xb2, 0xec, 0x46, 0xc9, 0xaa,
        0xa3, 0xca, 0x41, 0x33, 0xcc, 0xb4, 0x1a, 0xac,
        0x3d, 0x66, 0x0c, 0xa5, 0xb7, 0x79, 0xa6, 0x6e
    }
};

const SYMCRYPT_SELFTEST_DLKEY_2048 dhKey2 =
{
    // Public key
    {
        0x51, 0x7f, 0x01, 0xa6, 0xe7, 0x95, 0x2c, 0xb7,
        0x0e, 0x3c, 0x09, 0x6e, 0x31, 0xfb, 0x14, 0x87,
        0xaa, 0x27, 0x7f, 0xe0, 0x61, 0xed, 0xd7, 0x09,
        0x07, 0xb4, 0x40, 0xb8, 0xdf, 0xff, 0x05, 0x64,
        0x36, 0xc6, 0x0e, 0x50, 0x11, 0xd6, 0xac, 0x3f,
        0xc2, 0x5d, 0x52, 0x39, 0x44, 0xec, 0xdf, 0x86,
        0xde, 0x32, 0x9c, 0xde, 0x4e, 0x43, 0x30, 0x46,
        0x32, 0x68, 0x5b, 0xb6, 0x2c, 0xda, 0x04, 0x13,
        0x75, 0xa9, 0x80, 0x7a, 0x30, 0xf0, 0x83, 0xc4,
        0x94, 0xf7, 0x59, 0x3e, 0x91, 0xd1, 0x53, 0x74,
        0xde, 0xa6, 0x9a, 0xdf, 0x1d, 0x0a, 0xc7, 0x20,
        0x9e, 0xef, 0x38, 0x0b, 0xd6, 0x20, 0x50, 0x6e,
        0x5b, 0x5e, 0x66, 0x3d, 0xda, 0xd8, 0xbe, 0xea,
        0x50, 0xee, 0x18, 0x54, 0xa9, 0x87, 0x3c, 0x1f,
        0x38, 0x95, 0x27, 0xfd, 0xe2, 0xb6, 0x98, 0x9f,
        0x50, 0x44, 0x7f, 0x66, 0x79, 0x17, 0x3f, 0x20,
        0x45, 0xba, 0x9c, 0x43, 0x74, 0x0c, 0xda, 0x89,
        0x80, 0x4b, 0x93, 0x02, 0x1b, 0x10, 0x1c, 0x3c,
        0x20, 0x80, 0x1e, 0xa2, 0xc4, 0x08, 0x10, 0xd3,
        0xda, 0xf3, 0x78, 0xc0, 0x06, 0x47, 0xed, 0x0d,
        0xa4, 0xf8, 0x14, 0xeb, 0xd2, 0xbc, 0xf5, 0xe2,
        0x56, 0x41, 0x48, 0x6a, 0xd5, 0x90, 0xb6, 0x15,
        0x5d, 0x47, 0xc0, 0xb8, 0x10, 0x8b, 0xd5, 0x56,
        0x8f, 0x95, 0xc2, 0xde, 0x8c, 0x9e, 0xc8, 0xa9,
        0x9f, 0xfd, 0x6b, 0xff, 0x3e, 0xe8, 0x71, 0xf1,
        0xb8, 0x01, 0xf0, 0x36, 0x07, 0x5b, 0xe5, 0x0a,
        0xfc, 0xe4, 0x42, 0xab, 0x11, 0xd3, 0x91, 0x68,
        0x5f, 0xba, 0x61, 0xa9, 0x2f, 0x69, 0x55, 0x15,
        0x1d, 0xde, 0x7c, 0x2c, 0xc1, 0x05, 0x34, 0x13,
        0x84, 0x48, 0xe0, 0xce, 0xfd, 0xa9, 0xfe, 0xd9,
        0x76, 0x94, 0x07, 0xde, 0x0f, 0xe4, 0x98, 0x46,
        0xbc, 0x32, 0x3b, 0xd7, 0xce, 0x16, 0xae, 0x71
    },

    // Private key
    {
        0xad, 0xcb, 0x1d, 0xab, 0xa7, 0xc3, 0x78, 0xdb,
        0x37, 0x0f, 0x0b, 0x9c, 0xcd, 0x4f, 0x05, 0x5f,
        0x0e, 0x2b, 0xc3, 0xee, 0xa9, 0xb4, 0xd3, 0xb9,
        0x77, 0xd8, 0x89, 0xbd, 0xf6, 0x0e, 0xd7, 0x18
    }
};

// Shared secret generated from the two above keys
const BYTE rgbDhKnownSecret[] =
{
    0xa8, 0xf4, 0x48, 0xa2, 0x9f, 0xe0, 0xae, 0x75,
    0xf1, 0xec, 0x1f, 0x65, 0x93, 0x72, 0x25, 0xf2,
    0x3d, 0x9f, 0x8b, 0x74, 0xee, 0x9c, 0x60, 0x1c,
    0xae, 0x8c, 0x8f, 0x61, 0x62, 0xa7, 0xa9, 0xa0,
    0xaa, 0xe3, 0x85, 0x11, 0xcc, 0xe4, 0x97, 0x3d,
    0xbc, 0x59, 0x74, 0x57, 0x70, 0x45, 0xe9, 0x1f,
    0x82, 0x14, 0x8f, 0xb1, 0x51, 0x3c, 0x79, 0x51,
    0x06, 0xe8, 0xed, 0xc0, 0xe9, 0xd0, 0xde, 0xb0,
    0xd2, 0x06, 0xf1, 0x15, 0x81, 0x7a, 0x26, 0x80,
    0x89, 0x26, 0x77, 0xda, 0x50, 0xdd, 0x86, 0x90,
    0x82, 0x06, 0x35, 0x81, 0x82, 0xfd, 0x19, 0x25,
    0xdf, 0x7f, 0xa2, 0xde, 0xd0, 0x40, 0x2c, 0x5c,
    0x74, 0x5e, 0x0c, 0xbc, 0xea, 0x65, 0x7a, 0x7e,
    0xbc, 0x93, 0xf3, 0xdd, 0x62, 0x10, 0x68, 0x0b,
    0xb3, 0x7e, 0x5c, 0xd0, 0xd0, 0x74, 0x7e, 0xa5,
    0xfc, 0xd4, 0xc8, 0xdc, 0x00, 0xde, 0x10, 0x53,
    0x9b, 0xa0, 0x26, 0x6b, 0x57, 0xe6, 0x50, 0x69,
    0x03, 0x18, 0xef, 0xe9, 0x3d, 0xc1, 0x08, 0x74,
    0xdb, 0x1e, 0x02, 0x9d, 0x6c, 0x03, 0x04, 0xdc,
    0x0c, 0xad, 0x68, 0x23, 0x08, 0x6e, 0x9c, 0x8a,
    0x2b, 0xb8, 0xa3, 0xca, 0xf1, 0x3e, 0x16, 0x17,
    0x43, 0xa4, 0x64, 0xf3, 0x64, 0x75, 0x54, 0xdd,
    0x25, 0x19, 0x37, 0xf4, 0x7b, 0x14, 0x12, 0xa7,
    0x62, 0xa7, 0xc5, 0x27, 0x46, 0xd6, 0xda, 0xe8,
    0x4e, 0xcb, 0xf4, 0x80, 0x7e, 0x8c, 0xa6, 0xb4,
    0x52, 0x0a, 0x1d, 0xf6, 0x24, 0xf2, 0x13, 0xe3,
    0x4a, 0xcc, 0x86, 0x27, 0x26, 0x57, 0x06, 0xfa,
    0xfa, 0x9d, 0xb2, 0x4c, 0x0c, 0xa4, 0xad, 0x07,
    0x47, 0x13, 0xbb, 0x80, 0x63, 0x37, 0x37, 0x6c,
    0xa4, 0x2e, 0xb7, 0xd5, 0x74, 0x37, 0xb7, 0x95,
    0xe1, 0xc4, 0xbe, 0x25, 0x33, 0xcc, 0x0e, 0xdf,
    0x53, 0xa6, 0xf9, 0x6f, 0xbc, 0x7b, 0xa6, 0x5c
};

//
// DL group and keys for DSA selftest
//

const SYMCRYPT_SELFTEST_DLGROUP_2048 dsaDlgroup =
{
    // Prime P
    {
        0x8a, 0x09, 0xf1, 0x3b, 0xb4, 0xce, 0xc7, 0xb9,
        0x73, 0x36, 0xae, 0xb2, 0x45, 0xee, 0x9d, 0x7d,
        0x3b, 0xef, 0xaf, 0x78, 0xb7, 0x8c, 0x62, 0xdb,
        0xd1, 0x86, 0x60, 0xac, 0xef, 0xa6, 0x3f, 0x14,
        0x19, 0xa8, 0x02, 0xf4, 0xf3, 0xaa, 0x44, 0x91,
        0x4a, 0xc9, 0xa8, 0xf8, 0x70, 0xb0, 0x95, 0x96,
        0xbc, 0x52, 0x6c, 0x0c, 0x46, 0x42, 0x2c, 0x4a,
        0x12, 0xce, 0xfb, 0x28, 0x3d, 0x3a, 0x0c, 0x53,
        0x48, 0x8b, 0x6c, 0x2a, 0x43, 0x55, 0x64, 0xb2,
        0x60, 0x5e, 0x54, 0xdc, 0x72, 0x35, 0x33, 0xec,
        0x1b, 0xd6, 0x6d, 0x8f, 0xd5, 0xaf, 0x6d, 0x12,
        0xf0, 0x26, 0xef, 0x1d, 0xd7, 0x49, 0xf5, 0x5c,
        0xf6, 0xd8, 0xc8, 0x39, 0xcd, 0xb3, 0xc8, 0xa6,
        0xb1, 0x9c, 0xac, 0x52, 0x89, 0xce, 0xe4, 0x54,
        0xf7, 0x0e, 0xe8, 0xdc, 0xbc, 0x32, 0x75, 0x53,
        0xcf, 0xe8, 0xe3, 0x29, 0x03, 0xbf, 0xa1, 0x56,
        0x0d, 0xce, 0xfa, 0xd1, 0x8b, 0x55, 0x0c, 0xdd,
        0x94, 0xe2, 0xfa, 0xf0, 0x52, 0xaf, 0x4b, 0xcb,
        0x38, 0xb3, 0x6a, 0xb9, 0x5a, 0x75, 0x6f, 0xf2,
        0xbb, 0x5d, 0xcb, 0x47, 0x8a, 0xb0, 0x86, 0x85,
        0xdd, 0x49, 0x47, 0xbc, 0xa9, 0xa5, 0x7b, 0xd2,
        0xba, 0x0e, 0x23, 0x41, 0xea, 0x12, 0xb6, 0x0f,
        0x5e, 0x0b, 0xb0, 0x07, 0x63, 0x34, 0x65, 0x66,
        0x07, 0x98, 0xa1, 0x9a, 0x12, 0x75, 0x25, 0x2f,
        0xd2, 0x3b, 0x1e, 0x89, 0xb8, 0x70, 0xfd, 0x89,
        0xfd, 0x95, 0x17, 0x9a, 0xe3, 0xb8, 0xbb, 0x1c,
        0x93, 0x87, 0x4a, 0x3b, 0x5f, 0xdf, 0xca, 0x09,
        0xd7, 0xc6, 0xca, 0xaf, 0x7b, 0xcf, 0x03, 0x58,
        0x89, 0xb1, 0x1e, 0x4d, 0x5d, 0x89, 0x10, 0xa8,
        0x53, 0xf4, 0x12, 0x56, 0x8b, 0x85, 0xe2, 0xcc,
        0x02, 0x86, 0x82, 0x77, 0x2e, 0x0f, 0x30, 0x86,
        0x1b, 0x6e, 0xd1, 0xda, 0x15, 0x05, 0x5f, 0x93
    },

    // Prime Q
    {
        0xb8, 0x0e, 0xd4, 0x3a, 0xe1, 0x2a, 0x6a, 0xb7,
        0xd5, 0x6d, 0x3f, 0x8c, 0x86, 0xb9, 0xb9, 0x9d,
        0xb4, 0x28, 0xa3, 0x96, 0x61, 0xab, 0x19, 0x8a,
        0x92, 0xe1, 0xc2, 0xaa, 0x7e, 0x10, 0x03, 0x13
    },

    // Generator
    {
        0x1a, 0xe8, 0x38, 0xbe, 0xe7, 0xd0, 0xc5, 0x52,
        0x89, 0x0d, 0x7a, 0x45, 0x31, 0x83, 0x9a, 0xed,
        0xa5, 0x4e, 0x13, 0x15, 0xba, 0xb7, 0x01, 0xa1,
        0xf5, 0x86, 0x6a, 0x43, 0x88, 0x75, 0xdb, 0xed,
        0xe9, 0xa3, 0xa3, 0x43, 0x78, 0x4c, 0x01, 0x18,
        0x4d, 0x56, 0x3a, 0x49, 0x3c, 0xa7, 0x10, 0xba,
        0x0b, 0x1b, 0x11, 0x11, 0xef, 0x2d, 0xee, 0x12,
        0x76, 0x8c, 0xc2, 0xa8, 0xe2, 0x9d, 0xc6, 0x3f,
        0xb5, 0xe5, 0x91, 0x76, 0x0f, 0xd8, 0xf6, 0xff,
        0x59, 0x06, 0xb7, 0x4e, 0xcc, 0xb3, 0x75, 0x50,
        0x47, 0x6c, 0x61, 0xf6, 0x3f, 0xe7, 0x9f, 0x80,
        0x4c, 0x04, 0x84, 0xac, 0xfd, 0xab, 0x63, 0xa3,
        0x99, 0x61, 0x62, 0x26, 0x5d, 0x1c, 0xc3, 0x11,
        0x31, 0x2e, 0x4c, 0xf1, 0x81, 0xc6, 0x45, 0xd4,
        0x3a, 0x3d, 0x17, 0x48, 0x8e, 0x84, 0x38, 0xde,
        0xc1, 0x55, 0x42, 0xca, 0xf8, 0xb0, 0x76, 0x54,
        0x90, 0xac, 0xf8, 0x0f, 0x38, 0xdd, 0x81, 0x70,
        0xa9, 0xb8, 0x6f, 0xcb, 0x3e, 0xba, 0xf7, 0x1e,
        0x5d, 0xaa, 0xa0, 0xcb, 0x7c, 0x37, 0xa1, 0x87,
        0xb0, 0x34, 0x6e, 0x78, 0x62, 0x7e, 0x17, 0xe6,
        0xae, 0xc3, 0x4e, 0x0c, 0xeb, 0x4f, 0x25, 0x76,
        0x4a, 0xb6, 0xe5, 0xc6, 0x23, 0x77, 0xfb, 0xa8,
        0xb2, 0x0c, 0xa1, 0xc0, 0x56, 0xe9, 0x22, 0x2d,
        0x3f, 0x44, 0x43, 0x31, 0xdf, 0x2f, 0x5f, 0x57,
        0x22, 0x6e, 0x8a, 0xf1, 0x64, 0xb1, 0x3c, 0xd3,
        0x22, 0x78, 0x8a, 0xe3, 0x12, 0xbd, 0x47, 0x20,
        0xd3, 0x19, 0x22, 0xb7, 0xc7, 0xc7, 0x6a, 0xb8,
        0x7e, 0x13, 0x34, 0x13, 0x41, 0x47, 0x56, 0xd4,
        0xff, 0x4f, 0x37, 0xd2, 0x8d, 0x58, 0x8f, 0xe3,
        0xea, 0x04, 0x0a, 0x13, 0x3f, 0x3b, 0x8f, 0x71,
        0x5a, 0xbe, 0xf8, 0x90, 0x8d, 0x3b, 0xd5, 0xeb,
        0x97, 0x6f, 0xeb, 0xb1, 0xe1, 0xed, 0x1c, 0x6f
    },

    // Seed
    {
        0xe7, 0x53, 0x59, 0xd5, 0xca, 0x3a, 0x7c, 0xe1,
        0x83, 0x68, 0x7a, 0x79, 0xff, 0x65, 0xad, 0x40,
        0x38, 0x69, 0x59, 0x10, 0x5a, 0x18, 0x4d, 0xc5,
        0x3a, 0xd2, 0x7e, 0x95, 0x23, 0xa3, 0x7b, 0x3d
    },

    // Counter
    3394
};

const SYMCRYPT_SELFTEST_DLKEY_2048 dsaKey =
{
    // Public key
    {
    0x24, 0xaa, 0x5c, 0x6a, 0x72, 0x54, 0x3a, 0x40,
    0x8a, 0xd7, 0xa4, 0xd2, 0x59, 0x7d, 0xfc, 0x9d,
    0x93, 0xc3, 0x9f, 0x84, 0x50, 0x4e, 0x03, 0x17,
    0x87, 0x19, 0x35, 0xf0, 0x17, 0x17, 0x22, 0xb8,
    0x08, 0xca, 0xe4, 0x22, 0xd3, 0xb5, 0x52, 0x92,
    0xf0, 0xa9, 0x83, 0x45, 0xe4, 0x2c, 0x1f, 0xa7,
    0xb1, 0x9f, 0x8e, 0x35, 0xfe, 0x26, 0x1f, 0xe0,
    0xda, 0x74, 0xe4, 0xa4, 0xa0, 0xf6, 0x17, 0x0b,
    0x04, 0x86, 0xc7, 0x5e, 0x2e, 0xc0, 0x52, 0xfe,
    0x29, 0x12, 0x48, 0x3a, 0x16, 0x12, 0xfb, 0xbe,
    0xd9, 0x71, 0xf2, 0x65, 0x44, 0xef, 0x41, 0xf4,
    0x67, 0x7c, 0x8b, 0xeb, 0xb8, 0xb6, 0x58, 0x5d,
    0xd0, 0xbb, 0x42, 0xf9, 0xc7, 0x32, 0xcc, 0x44,
    0x46, 0x93, 0x8f, 0x2e, 0xe2, 0xd7, 0x6c, 0xa6,
    0x53, 0x4d, 0xc9, 0x12, 0xbb, 0xc9, 0xee, 0xb2,
    0xba, 0xc0, 0x0e, 0xc2, 0x7e, 0x61, 0x61, 0x29,
    0x2d, 0x70, 0x70, 0x86, 0x10, 0xd8, 0x24, 0x6d,
    0x6e, 0x4c, 0x7d, 0xb2, 0xc9, 0x10, 0x0a, 0x3c,
    0xf8, 0x46, 0x91, 0xeb, 0xb7, 0xc2, 0x31, 0x32,
    0xf9, 0x5b, 0x29, 0x53, 0x38, 0x75, 0x27, 0x88,
    0x53, 0x2a, 0xe2, 0xae, 0x61, 0x7d, 0xe0, 0xd1,
    0xb1, 0x9a, 0x43, 0xcf, 0xf1, 0x91, 0x19, 0x11,
    0x4d, 0x7c, 0xf8, 0x5a, 0x37, 0x9c, 0x2b, 0x0c,
    0x40, 0x74, 0x40, 0xee, 0xef, 0x5b, 0x1f, 0x3b,
    0xa1, 0xb8, 0x06, 0x2e, 0x5d, 0x1d, 0x23, 0xd8,
    0x05, 0x08, 0xc4, 0x1a, 0x70, 0x96, 0x96, 0x29,
    0x6a, 0x12, 0x3a, 0x96, 0x68, 0x94, 0x8e, 0x3a,
    0xfe, 0x2d, 0x71, 0x0c, 0x54, 0x28, 0x67, 0xbf,
    0x95, 0x69, 0x68, 0x73, 0xee, 0xe6, 0x5d, 0x65,
    0x79, 0x57, 0xe5, 0x45, 0x08, 0xbd, 0xf1, 0x8c,
    0x59, 0x45, 0x97, 0xa9, 0x43, 0xd6, 0xbd, 0xb5,
    0x34, 0xf0, 0x6a, 0x0a, 0x52, 0x6e, 0x47, 0xa6
    },

    // Private key
    {
        0x0f, 0x3f, 0xab, 0x80, 0xf1, 0x29, 0x5d, 0x41,
        0xf7, 0xbb, 0xff, 0xa6, 0x0a, 0x28, 0x9d, 0x46,
        0x56, 0x28, 0x7c, 0x9b, 0x3e, 0x4a, 0x06, 0x95,
        0x7d, 0xea, 0x04, 0x1e, 0xab, 0x9a, 0x78, 0x17
    }
};

//
// ECDH/ECDSA keys
//

const SYMCRYPT_SELFTEST_ECKEY_P256 eckey1 =
{
    // Qx
    {
        0xdd, 0xd5, 0x15, 0x20, 0x43, 0x8d, 0x41, 0xa9,
        0x18, 0xcf, 0x62, 0xc2, 0x13, 0xf7, 0xed, 0xb2,
        0xf9, 0x8f, 0x02, 0xa3, 0x78, 0x30, 0x7e, 0x22,
        0x8f, 0xc1, 0x44, 0xbe, 0xde, 0xc6, 0x65, 0x91
    },
    //Qy
    {
        0x72, 0xad, 0x17, 0xad, 0x51, 0x8c, 0xd3, 0x60,
        0x0f, 0x54, 0xc0, 0xf4, 0xc3, 0x22, 0x5b, 0x44,
        0xab, 0xad, 0x28, 0xb5, 0x56, 0x8e, 0x78, 0x0a,
        0x6a, 0x09, 0x6b, 0x65, 0x81, 0x6d, 0x6f, 0x99
    },
    //d
    {
        0x07, 0x36, 0x9f, 0xb2, 0x35, 0xce, 0xe2, 0xd4,
        0x7e, 0x13, 0x35, 0x31, 0xae, 0xa5, 0x6e, 0x6c,
        0x96, 0xd3, 0x9f, 0x3b, 0xa7, 0x74, 0xae, 0xf9,
        0x7a, 0x56, 0x6e, 0xfe, 0x32, 0x3f, 0x43, 0xaa
    },
};

const SYMCRYPT_SELFTEST_ECKEY_P256 eckey2 =
{
    //Qx
    {
        0x21, 0xf2, 0xf7, 0x08, 0x8c, 0x71, 0x59, 0xa7,
        0x0c, 0xe1, 0xb9, 0x1a, 0xe0, 0xed, 0x69, 0xbe,
        0x44, 0xeb, 0xa3, 0x51, 0xfd, 0x32, 0x4a, 0x90,
        0xdc, 0xde, 0xa4, 0x10, 0xe4, 0x44, 0x69, 0x29
    },
    //Qy
    {
        0x74, 0xd0, 0xc6, 0xbd, 0xe5, 0x13, 0x68, 0x07,
        0x9f, 0x40, 0x5e, 0xbf, 0x9e, 0x61, 0x7c, 0x3f,
        0xc8, 0x16, 0xe2, 0xd5, 0x0e, 0xf8, 0x09, 0x15,
        0xf3, 0x30, 0xba, 0x45, 0x25, 0xab, 0x9a, 0xae
    },
    //d
    {
        0xd0, 0x93, 0xf2, 0x34, 0x82, 0x39, 0xa6, 0x5c,
        0xd7, 0xe5, 0x10, 0x27, 0x0f, 0xfc, 0x0a, 0x0d,
        0x89, 0x97, 0x10, 0xa7, 0x50, 0x5a, 0xc4, 0x1b,
        0x5d, 0x18, 0x03, 0x2f, 0x7d, 0x46, 0x58, 0x4d
    }
};

// Shared secret generated from the two keys above
BYTE rgbEcdhKnownSecret[] =
{
    0x16, 0x94, 0xc8, 0xb3, 0xe9, 0xbe, 0x65, 0x41,
    0x75, 0xba, 0x71, 0x5d, 0x0a, 0xab, 0x6f, 0x6d,
    0xeb, 0xdb, 0x70, 0x75, 0xd1, 0x9e, 0x90, 0x7f,
    0xb6, 0x08, 0x32, 0x29, 0x34, 0x40, 0x1e, 0xd4
};

//
// Key from http://csrc.nist.gov/cryptval/dss/RSAExample.zip.
//
const SYMCRYPT_SELFTEST_RSAKEY_2048 rsakey =
{
    // publicExp
    0x3,
    // modulus
    {
        0x8f, 0xf3, 0x13, 0xb3, 0xad, 0xd8, 0x83, 0xd8,
        0x82, 0x2f, 0x46, 0xe1, 0x5e, 0x43, 0x6e, 0x38,
        0x7d, 0xa7, 0x84, 0xd3, 0x5e, 0xb4, 0x91, 0x2b,
        0x87, 0x89, 0x95, 0x30, 0x0b, 0x04, 0xdd, 0xe7,
        0x21, 0x6d, 0x24, 0xaf, 0xbe, 0x57, 0x0a, 0x0d,
        0x6a, 0x10, 0xd9, 0xa1, 0xf6, 0x02, 0x03, 0xd1,
        0x7e, 0x4d, 0xdb, 0xda, 0xa4, 0x96, 0x4c, 0x61,
        0x41, 0xfb, 0xbd, 0xc7, 0x2c, 0xd9, 0x30, 0xdb,
        0x0a, 0x43, 0x6d, 0x41, 0xce, 0x28, 0xad, 0xaf,
        0xbe, 0x94, 0x55, 0x39, 0x95, 0x94, 0xfc, 0x9a,
        0x8b, 0x80, 0x25, 0x4b, 0xf7, 0xdc, 0x8c, 0xfb,
        0xce, 0x92, 0xcf, 0x7a, 0xd7, 0x5e, 0x0f, 0x12,
        0x5d, 0xca, 0x5a, 0xaf, 0x94, 0xcf, 0x13, 0x99,
        0x7c, 0xb2, 0x71, 0x81, 0x73, 0x4c, 0xbd, 0x56,
        0x7c, 0x55, 0xc3, 0x73, 0xa4, 0x74, 0xac, 0xb8,
        0xb5, 0x6f, 0xdd, 0x54, 0x3b, 0x97, 0x8a, 0x3a,
        0x72, 0xe0, 0xb3, 0x8d, 0x5a, 0xb4, 0xd8, 0x54,
        0x3d, 0xc1, 0x9f, 0x69, 0x7f, 0xc0, 0x30, 0x1e,
        0xd6, 0xb5, 0x4f, 0xcd, 0xe5, 0xdd, 0x68, 0x08,
        0x93, 0x92, 0x1f, 0x9f, 0xe3, 0x23, 0x84, 0x12,
        0xc9, 0xbb, 0xa5, 0xb8, 0x53, 0x14, 0x8b, 0xe0,
        0xa4, 0x93, 0x97, 0x89, 0x05, 0x16, 0xb2, 0xc8,
        0x70, 0x2a, 0xc8, 0x46, 0xcd, 0x92, 0x43, 0xad,
        0x6a, 0x35, 0x3e, 0x84, 0xcc, 0xbf, 0xa7, 0xfe,
        0x79, 0x9e, 0xf4, 0xc3, 0x7d, 0xde, 0x97, 0xa0,
        0x25, 0x7f, 0x93, 0x9c, 0xbc, 0x14, 0xce, 0xd6,
        0xd8, 0x6c, 0x47, 0x0f, 0xc9, 0x88, 0xd4, 0x35,
        0x05, 0x87, 0x83, 0xf8, 0xcb, 0xd8, 0xa6, 0x7e,
        0xd2, 0xd8, 0xcd, 0xe7, 0x1c, 0x6e, 0x74, 0xdd,
        0xc6, 0x49, 0xe5, 0x6f, 0x51, 0xc6, 0x29, 0x2a,
        0x15, 0xc1, 0xff, 0xbd, 0x85, 0x6e, 0xe1, 0x5f,
        0x49, 0xda, 0xcc, 0xbb, 0x74, 0x2a, 0x8a, 0xb9
    },
    // prime1
    {
        0xc8, 0x23, 0xbd, 0x5e, 0x64, 0x04, 0xba, 0xc4,
        0x51, 0x1d, 0x15, 0x05, 0xdc, 0xd1, 0x33, 0x3c,
        0xc3, 0xbe, 0xca, 0x9a, 0x7f, 0x43, 0x50, 0x0b,
        0x61, 0x3f, 0xb4, 0x16, 0xe0, 0x1c, 0x4b, 0x05,
        0x32, 0x0d, 0x0d, 0x47, 0x19, 0x93, 0x01, 0x12,
        0x00, 0x46, 0xf6, 0x4d, 0x46, 0xd2, 0x74, 0xac,
        0xda, 0x40, 0xf2, 0x1d, 0x08, 0x77, 0x49, 0x63,
        0x92, 0x02, 0x69, 0x00, 0x03, 0x13, 0x4b, 0x59,
        0x40, 0xc8, 0x27, 0x15, 0x60, 0xae, 0xe6, 0xc6,
        0xbd, 0x84, 0xda, 0xcb, 0x4b, 0x41, 0xad, 0x75,
        0x29, 0x58, 0x97, 0x68, 0x76, 0x3a, 0xd4, 0x44,
        0x66, 0x1c, 0x56, 0x85, 0x95, 0xfb, 0xb7, 0xa8,
        0x9b, 0xde, 0x71, 0x90, 0xe0, 0x63, 0x64, 0xdf,
        0xff, 0xc6, 0xe5, 0x2d, 0x5d, 0x87, 0x73, 0x94,
        0x61, 0x89, 0x23, 0x3b, 0x8f, 0x38, 0xdf, 0x67,
        0x14, 0x2b, 0x0a, 0x79, 0x61, 0x2d, 0xed, 0x0f
    },
    // prime2
    {
        0xb8, 0x20, 0x79, 0xad, 0x07, 0x09, 0x04, 0x80,
        0x78, 0x39, 0x24, 0x4a, 0xd8, 0x1f, 0xa3, 0x82,
        0x51, 0x8c, 0x18, 0x31, 0x5e, 0x5c, 0x00, 0xae,
        0x82, 0x58, 0x10, 0xac, 0x3a, 0x5b, 0xca, 0x77,
        0x30, 0xb4, 0x4f, 0x7b, 0xb1, 0xcc, 0x74, 0x06,
        0x61, 0x09, 0x17, 0x4f, 0x2e, 0xf1, 0xb4, 0xb6,
        0x82, 0x93, 0x33, 0x4d, 0xbd, 0x22, 0x04, 0x98,
        0xe2, 0xe4, 0x6e, 0x91, 0x90, 0x4a, 0x24, 0xef,
        0x35, 0x61, 0x1a, 0xa8, 0x7e, 0x69, 0x96, 0xb5,
        0x12, 0xc2, 0x8a, 0x15, 0x7f, 0x10, 0x21, 0x14,
        0x74, 0x97, 0x98, 0xcd, 0x32, 0xe7, 0x4f, 0xee,
        0x3d, 0xbe, 0x20, 0xbb, 0x57, 0x2a, 0xef, 0x56,
        0xae, 0xee, 0x82, 0x3d, 0x55, 0x35, 0xb3, 0x87,
        0x46, 0x96, 0x01, 0xd7, 0x83, 0x40, 0xfd, 0x94,
        0x0d, 0x2f, 0xe3, 0xe0, 0x92, 0x42, 0x02, 0xe7,
        0xbf, 0x02, 0xc1, 0x0f, 0xd1, 0x52, 0x9b, 0xb7
    }
};

// SHA256 hash for DSA, ECDSA, RSA sign/verify tests
// Hashed from: { 0x61, 0x62, 0x63 }
const BYTE rgbSha256Hash[] =
{
   0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 
   0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23,
   0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c,
   0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad
};

VOID
SYMCRYPT_CALL
SymCryptDhSecretAgreementSelftest()
{
    SYMCRYPT_ERROR scError = SYMCRYPT_NO_ERROR;

    PSYMCRYPT_DLGROUP pDlgroup = NULL;
    PSYMCRYPT_DLKEY pkKey1 = NULL;
    PSYMCRYPT_DLKEY pkKey2 = NULL;

    BYTE rgbSecret[sizeof(dhKey1.public)];

    pDlgroup = SymCryptDlgroupAllocate( sizeof(dhKey1.public) * 8, 0 );
    SYMCRYPT_FIPS_ASSERT(pDlgroup != NULL);

    scError = SymCryptDlgroupSetValue(
        SymCryptDlgroupDhSafePrimeParamsModp2048->pcbPrimeP,
        SymCryptDlgroupDhSafePrimeParamsModp2048->nBitsOfP / 8,
        NULL, // pbPrimeQ
        0,  // cbPrimeQ
        (PBYTE) &dhGenerator,
        sizeof(dhGenerator),
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        NULL, // pHashAlgorithm
        NULL, // pbSeed
        0, // cbSeed
        0, // genCounter
        SYMCRYPT_DLGROUP_FIPS_NONE,
        pDlgroup);
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    pkKey1 = SymCryptDlkeyAllocate( pDlgroup );
    SYMCRYPT_FIPS_ASSERT( pkKey1 != NULL );

    scError = SymCryptDlkeySetValue(
        dhKey1.private,
        sizeof(dhKey1.private),
        dhKey1.public,
        sizeof(dhKey1.public),
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_FLAG_KEY_RANGE_AND_PUBLIC_KEY_ORDER_VALIDATION | 
            SYMCRYPT_FLAG_KEY_KEYPAIR_REGENERATION_VALIDATION |
            SYMCRYPT_FLAG_BYPASS_FIPS_SELFTEST,
        pkKey1 );
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    pkKey2 = SymCryptDlkeyAllocate( pDlgroup );
    SYMCRYPT_FIPS_ASSERT( pkKey2 != NULL );

    scError = SymCryptDlkeySetValue(
        dhKey2.private,
        sizeof(dhKey2.private),
        dhKey2.public,
        sizeof(dhKey2.public),
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_FLAG_KEY_RANGE_AND_PUBLIC_KEY_ORDER_VALIDATION | 
            SYMCRYPT_FLAG_KEY_KEYPAIR_REGENERATION_VALIDATION |
            SYMCRYPT_FLAG_BYPASS_FIPS_SELFTEST,
        pkKey2 );
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    // Calculate secret using private key 1 and public key 2
    scError = SymCryptDhSecretAgreement(
        pkKey1,
        pkKey2,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_FLAG_BYPASS_FIPS_SELFTEST,
        rgbSecret,
        sizeof(rgbSecret) );
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    SYMCRYPT_FIPS_ASSERT( sizeof(rgbSecret) == sizeof(rgbDhKnownSecret) );
    SYMCRYPT_FIPS_ASSERT( memcmp( rgbSecret, rgbDhKnownSecret, sizeof(rgbDhKnownSecret) ) == 0 );

    SymCryptDlkeyFree( pkKey2 );
    SymCryptDlkeyFree( pkKey1 );
    SymCryptDlgroupFree( pDlgroup );
}

VOID
SYMCRYPT_CALL
SymCryptEcDhSecretAgreementSelftest( )
{
    SYMCRYPT_ERROR scError = SYMCRYPT_NO_ERROR;

    PSYMCRYPT_ECURVE pCurve = NULL;
    PSYMCRYPT_ECKEY pkKey1 = NULL;
    PSYMCRYPT_ECKEY pkKey2 = NULL;

    BYTE rgbSecret[ sizeof(rgbEcdhKnownSecret) ];
    UINT32 cbSecret = 0;

    pCurve = SymCryptEcurveAllocate( SymCryptEcurveParamsNistP256, 0 );
    SYMCRYPT_FIPS_ASSERT( pCurve != NULL );

    pkKey1 = SymCryptEckeyAllocate( pCurve );
    SYMCRYPT_FIPS_ASSERT( pkKey1 != NULL );

    scError = SymCryptEckeySetValue(
        eckey1.d,
        sizeof(eckey1.d),
        eckey1.Qx,
        sizeof(eckey1.Qx) + sizeof(eckey1.Qy),
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_ECPOINT_FORMAT_XY,
        SYMCRYPT_FLAG_KEY_RANGE_AND_PUBLIC_KEY_ORDER_VALIDATION | 
            SYMCRYPT_FLAG_KEY_KEYPAIR_REGENERATION_VALIDATION |
            SYMCRYPT_FLAG_BYPASS_FIPS_SELFTEST,
        pkKey1);
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    pkKey2 = SymCryptEckeyAllocate( pCurve );
    SYMCRYPT_FIPS_ASSERT( pkKey2 != NULL );

    scError = SymCryptEckeySetValue(
        eckey2.d,
        sizeof(eckey2.d),
        eckey2.Qx,
        sizeof(eckey2.Qx) + sizeof(eckey2.Qy),
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_ECPOINT_FORMAT_XY,
        SYMCRYPT_FLAG_KEY_RANGE_AND_PUBLIC_KEY_ORDER_VALIDATION | 
            SYMCRYPT_FLAG_KEY_KEYPAIR_REGENERATION_VALIDATION |
            SYMCRYPT_FLAG_BYPASS_FIPS_SELFTEST,
        pkKey2);
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    // Calculate secret using private key 1 and public key 2
    scError = SymCryptEcDhSecretAgreement(
        pkKey1,
        pkKey2,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_FLAG_BYPASS_FIPS_SELFTEST,
        rgbSecret,
        sizeof(rgbSecret));
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    SYMCRYPT_FIPS_ASSERT( memcmp(rgbSecret, rgbEcdhKnownSecret, sizeof(rgbSecret)) == 0);

    SymCryptEckeyFree( pkKey2 );
    SymCryptEckeyFree( pkKey1 );
    SymCryptEcurveFree( pCurve );
}

VOID
SYMCRYPT_CALL
SymCryptDsaSelftest( )
{
    SYMCRYPT_ERROR scError = SYMCRYPT_NO_ERROR;

    PSYMCRYPT_DLGROUP pDlgroup = NULL;
    PSYMCRYPT_DLKEY pkDlkey = NULL;

    BYTE rgbSignature[2 * sizeof(dsaKey.private)];

    pDlgroup = SymCryptDlgroupAllocate(
        sizeof(dsaDlgroup.primeP) * 8,
        sizeof(dsaDlgroup.primeQ) * 8);
    SYMCRYPT_FIPS_ASSERT(pDlgroup != NULL);

    scError = SymCryptDlgroupSetValue(
        dsaDlgroup.primeP,
        sizeof(dsaDlgroup.primeP),
        dsaDlgroup.primeQ,
        sizeof(dsaDlgroup.primeQ),
        dsaDlgroup.generator,
        sizeof(dsaDlgroup.generator),
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SymCryptSha256Algorithm,
        dsaDlgroup.seed,
        sizeof(dsaDlgroup.seed),
        dsaDlgroup.counter,
        SYMCRYPT_DLGROUP_FIPS_LATEST,
        pDlgroup);
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    pkDlkey = SymCryptDlkeyAllocate( pDlgroup );
    SYMCRYPT_FIPS_ASSERT( pkDlkey != NULL );

    scError = SymCryptDlkeySetValue(
        dsaKey.private,
        sizeof(dsaKey.private),
        dsaKey.public,
        sizeof(dsaKey.public),
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_FLAG_KEY_RANGE_AND_PUBLIC_KEY_ORDER_VALIDATION | 
            SYMCRYPT_FLAG_KEY_KEYPAIR_REGENERATION_VALIDATION |
            SYMCRYPT_FLAG_BYPASS_FIPS_SELFTEST,
        pkDlkey );
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    scError = SymCryptDsaSign(
        pkDlkey,
        rgbSha256Hash,
        sizeof(rgbSha256Hash),
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        0,
        rgbSignature,
        sizeof(rgbSignature) );
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    scError = SymCryptDsaVerify(
        pkDlkey,
        rgbSha256Hash,
        sizeof(rgbSha256Hash),
        rgbSignature,
        sizeof(rgbSignature),
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        0 );
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    SymCryptDlkeyFree( pkDlkey );
    SymCryptDlgroupFree( pDlgroup );
}

VOID
SYMCRYPT_CALL
SymCryptEcDsaSelftest( )
{
    SYMCRYPT_ERROR scError = SYMCRYPT_NO_ERROR;

    PSYMCRYPT_ECURVE pCurve = NULL;
    PSYMCRYPT_ECKEY pkEckey = NULL;

    BYTE rgbSignature[2 * sizeof(eckey1.d)];

    pCurve = SymCryptEcurveAllocate( SymCryptEcurveParamsNistP256, 0 );
    SYMCRYPT_FIPS_ASSERT( pCurve != NULL );

    pkEckey = SymCryptEckeyAllocate( pCurve );
    SYMCRYPT_FIPS_ASSERT( pkEckey != NULL );

    scError = SymCryptEckeySetValue(
        eckey1.d,
        sizeof(eckey1.d),
        eckey1.Qx,
        sizeof(eckey1.Qx) + sizeof(eckey1.Qy),
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_ECPOINT_FORMAT_XY,
        SYMCRYPT_FLAG_KEY_RANGE_AND_PUBLIC_KEY_ORDER_VALIDATION | 
            SYMCRYPT_FLAG_KEY_KEYPAIR_REGENERATION_VALIDATION |
            SYMCRYPT_FLAG_BYPASS_FIPS_SELFTEST,
        pkEckey);
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    scError = SymCryptEcDsaSign(
        pkEckey,
        rgbSha256Hash,
        sizeof(rgbSha256Hash),
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        0,
        rgbSignature,
        sizeof(rgbSignature) );
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    scError = SymCryptEcDsaVerify(
        pkEckey,
        rgbSha256Hash,
        sizeof(rgbSha256Hash),
        rgbSignature,
        sizeof(rgbSignature),
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        0 );
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR )

    SymCryptEckeyFree( pkEckey );
    SymCryptEcurveFree( pCurve );
}

VOID
SYMCRYPT_CALL
SymCryptRsaSelftest( )
{
    SYMCRYPT_ERROR scError = SYMCRYPT_NO_ERROR;

    PSYMCRYPT_RSAKEY pkRsakey = NULL;
    SYMCRYPT_RSA_PARAMS rsaParams = { 0 };

    rsaParams.version = 1;
    rsaParams.nBitsOfModulus = sizeof(rsakey.modulus) * 8;
    rsaParams.nPrimes = 2;
    rsaParams.nPubExp = 1;

    BYTE rgbSignature[sizeof(rsakey.modulus)];
    SIZE_T cbSignature = sizeof(rgbSignature);

    PCBYTE pbPrimes[] = { rsakey.prime1, rsakey.prime2 };
    SIZE_T cbPrimes[] = { sizeof(rsakey.prime1), sizeof(rsakey.prime2) };

    pkRsakey = SymCryptRsakeyAllocate( &rsaParams, 0 );
    SYMCRYPT_FIPS_ASSERT( pkRsakey != NULL );

    scError = SymCryptRsakeySetValue(
        rsakey.modulus,
        sizeof(rsakey.modulus),
        &rsakey.publicExp,
        1,
        pbPrimes,
        cbPrimes,
        sizeof(cbPrimes) / sizeof(cbPrimes[0]),
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_FLAG_BYPASS_FIPS_SELFTEST,
        pkRsakey );
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    scError = SymCryptRsaPkcs1Sign(
        pkRsakey,
        rgbSha256Hash,
        sizeof(rgbSha256Hash),
        SymCryptSha256OidList,
        SYMCRYPT_SHA256_OID_COUNT,
        0,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        rgbSignature,
        cbSignature,
        &cbSignature );
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    scError = SymCryptRsaPkcs1Verify(
        pkRsakey,
        rgbSha256Hash,
        sizeof(rgbSha256Hash),
        rgbSignature,
        cbSignature,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SymCryptSha256OidList,
        SYMCRYPT_SHA256_OID_COUNT,
        0 );
    SYMCRYPT_FIPS_ASSERT( scError == SYMCRYPT_NO_ERROR );

    SymCryptRsakeyFree( pkRsakey );
}