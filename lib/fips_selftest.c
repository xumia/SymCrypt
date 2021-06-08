//
// fips_selftest.c - Algorithm selftests for FIPS 140 compliance
//
// Copyright (c) Microsoft Corporation. Licensed under the MIT license.
//

#include "precomp.h"

SYMCRYPT_FIPS_SELFTEST g_SymCryptFipsSelftestsPerformed = SYMCRYPT_SELFTEST_NONE;

// 0x44, 0x48, 0x50, 0x56, // magic
// 0x00, 0x01, 0x00, 0x00, // key length

const unsigned char rgbDh2048Modulus[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34,
    0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74,
    0x02, 0x0B, 0xBE, 0xA6, 0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
    0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D, 0xF2, 0x5F, 0x14, 0x37,
    0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45, 0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6,
    0xF4, 0x4C, 0x42, 0xE9, 0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
    0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11, 0x7C, 0x4B, 0x1F, 0xE6,
    0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D, 0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05,
    0x98, 0xDA, 0x48, 0x36, 0x1C, 0x55, 0xD3, 0x9A, 0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F,
    0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96, 0x1C, 0x62, 0xF3, 0x56, 0x20, 0x85, 0x52, 0xBB,
    0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D, 0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04,
    0xF1, 0x74, 0x6C, 0x08, 0xCA, 0x18, 0x21, 0x7C, 0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B,
    0xE3, 0x9E, 0x77, 0x2C, 0x18, 0x0E, 0x86, 0x03, 0x9B, 0x27, 0x83, 0xA2, 0xEC, 0x07, 0xA2, 0x8F,
    0xB5, 0xC5, 0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9, 0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18,
    0x39, 0x95, 0x49, 0x7C, 0xEA, 0x95, 0x6A, 0xE5, 0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10,
    0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAC, 0xAA, 0x68, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const unsigned char rgbDh2048Generator[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
};

const unsigned char rgbDh2048Public[] = {
    0xC7, 0xCF, 0x6A, 0xA3, 0x34, 0x61, 0x7A, 0x6B, 0x65, 0x2E, 0xE3, 0x73, 0xD8, 0x59, 0x5E, 0x0C,
    0x35, 0x7D, 0x22, 0x5C, 0xC3, 0x9E, 0x3E, 0xAC, 0x81, 0x85, 0x8C, 0x19, 0x86, 0xCC, 0xB4, 0x33,
    0x26, 0x72, 0x6B, 0xD3, 0xE7, 0x6F, 0x99, 0x7A, 0x65, 0x28, 0x10, 0xC9, 0x75, 0xF2, 0x57, 0x45,
    0xD3, 0x4A, 0x9E, 0x00, 0xB7, 0x8F, 0xA7, 0xDA, 0xBF, 0x1D, 0x5C, 0xBD, 0xF6, 0xEA, 0xAB, 0x66,
    0x25, 0x26, 0x74, 0x67, 0xC5, 0x79, 0xEA, 0x69, 0x56, 0x89, 0x89, 0xCE, 0x11, 0x9F, 0xA1, 0xE3,
    0x0B, 0x61, 0x63, 0x10, 0x89, 0x75, 0x0D, 0xAB, 0x1A, 0x59, 0x5E, 0xEF, 0xDC, 0xDD, 0x24, 0x49,
    0xF3, 0x0C, 0x13, 0x04, 0x60, 0x4C, 0xCD, 0x25, 0x82, 0x07, 0xD5, 0x9B, 0x7A, 0x99, 0x86, 0x86,
    0x4F, 0xC6, 0x43, 0x87, 0x55, 0xB2, 0x52, 0x90, 0x8B, 0xB3, 0xDA, 0x71, 0x51, 0x8D, 0x5A, 0xBD,
    0x24, 0xE1, 0x4E, 0x9B, 0xDF, 0xBB, 0xAD, 0x0F, 0xFF, 0x4A, 0xE8, 0xFA, 0xD8, 0xE7, 0x52, 0x10,
    0x65, 0x91, 0x61, 0x0D, 0x09, 0x09, 0xE2, 0x2E, 0x20, 0xC9, 0x0C, 0x76, 0x61, 0x5E, 0xA7, 0xDA,
    0xD0, 0x08, 0x7A, 0xD7, 0x4F, 0xAD, 0x37, 0x57, 0x45, 0x92, 0x38, 0x83, 0x17, 0xF1, 0x04, 0xA9,
    0x38, 0x45, 0xD2, 0xA6, 0xC0, 0xDA, 0x2B, 0xD0, 0xBA, 0x81, 0xD6, 0xF0, 0x2A, 0x0F, 0x03, 0xD6,
    0xD4, 0x31, 0x54, 0x49, 0x0D, 0x87, 0x04, 0x53, 0x51, 0xED, 0xF1, 0x96, 0x65, 0xEA, 0xA1, 0x28,
    0x60, 0x9D, 0xB7, 0x50, 0xA8, 0x66, 0x22, 0x70, 0x74, 0x51, 0x28, 0x81, 0xB2, 0xCA, 0x37, 0x83,
    0xDC, 0x55, 0x4D, 0xF2, 0xBB, 0xBB, 0xD6, 0x33, 0xA4, 0xD4, 0x2A, 0x01, 0xEF, 0xD8, 0xDA, 0xC2,
    0xE1, 0x20, 0xD8, 0xAE, 0x41, 0xEF, 0x5A, 0x63, 0x0D, 0x2B, 0x05, 0xD2, 0x21, 0xCC, 0x2A, 0xCC
};

const unsigned char rgbDh2048Private[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x04, 0x5A, 0x39, 0xD6, 0x42, 0xB3, 0xEE, 0xD0, 0x12, 0x2C, 0x47, 0xB3, 0x52, 0xC1, 0xC5, 0x00,
    0x21, 0xAD, 0x3F, 0x1B, 0xFA, 0x3D, 0x9A, 0xD3, 0x6D, 0x28, 0x5C, 0xDE, 0x08, 0xA5, 0xA5, 0xF6,
    0xCA, 0xD0, 0x22, 0xD1, 0x4F, 0x85, 0xF4, 0x49, 0x9F, 0x04, 0x84, 0x8D, 0x72, 0xEA, 0xD8, 0x3F,
    0xE4, 0x2E, 0xE1, 0xF8, 0xE4, 0x7C, 0x4F, 0xDC, 0x16, 0xF6, 0x3D, 0x28, 0x2E, 0x79, 0x8F, 0xC5
};

SYMCRYPT_ERROR
SYMCRYPT_CALL
SymCryptDhSecretAgreementSelftest(
    _In_ PCSYMCRYPT_DLKEY pkCallerKeyPrivate )
{
    SYMCRYPT_ERROR scError = SYMCRYPT_NO_ERROR;

    PSYMCRYPT_DLKEY pkTestKeyPrivate = NULL;
    PSYMCRYPT_DLKEY pkCallerKeyPublic = NULL;
    PSYMCRYPT_DLKEY pkTestKeyPublic = NULL;

    PBYTE pbKeyBufferPublic = NULL;
    UINT32 cbKeyBufferPublic = 0;

    PBYTE pbSecret1 = NULL;
    PBYTE pbSecret2 = NULL;

    // Export the caller public key and import it into a new Dlkey so we can do secret
    // agreement with (caller public key, test private key)
    cbKeyBufferPublic = SymCryptDlkeySizeofPublicKey( pkCallerKeyPrivate );
    pbKeyBufferPublic = SymCryptCallbackAlloc( cbKeyBufferPublic );
    if( pbKeyBufferPublic == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    scError = SymCryptDlkeyGetValue(
        pkCallerKeyPrivate,
        NULL, // pbPrivateKey
        0, // cbPrivateKey
        pbKeyBufferPublic,
        cbKeyBufferPublic,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        0 );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    pkCallerKeyPublic = SymCryptDlkeyAllocate( pkCallerKeyPrivate->pDlgroup );
    if( pkCallerKeyPublic == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    scError = SymCryptDlkeySetValue(
        NULL, // pbPrivateKey
        0, // cbPrivateKey
        pbKeyBufferPublic,
        cbKeyBufferPublic,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        0,
        pkCallerKeyPublic );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    // Import or generate a test private key. This key must be the same size as the caller key,
    // so if our pre-generated key doesn't match, we have to generate a new one, which is expensive.
    pkTestKeyPrivate = SymCryptDlkeyAllocate( pkCallerKeyPrivate->pDlgroup );
    if( pkTestKeyPrivate == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    // if( cbKeyBufferPublic == sizeof(rgbDh2048Public) )
    // {
    //     scError = SymCryptDlkeySetValue(
    //         rgbDh2048Private,
    //         sizeof(rgbDh2048Private),
    //         rgbDh2048Public,
    //         sizeof(rgbDh2048Public),
    //         SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
    //         SYMCRYPT_FLAG_KEY_RANGE_AND_PUBLIC_KEY_ORDER_VALIDATION,
    //         pkTestKeyPrivate );
    //     if( scError != SYMCRYPT_NO_ERROR )
    //     {
    //         goto cleanup;
    //     }
    // }
    // else
    {
        scError = SymCryptDlkeyGenerate( 0, pkTestKeyPrivate );
        if( scError != SYMCRYPT_NO_ERROR )
        {
            goto cleanup;
        }
    }

    // We ensured that our key sizes are the same, so no need to reallocate this buffer
    scError = SymCryptDlkeyGetValue(
        pkTestKeyPrivate,
        NULL, // pbPrivateKey
        0, // cbPrivateKey
        pbKeyBufferPublic,
        cbKeyBufferPublic,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        0 );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    // Allocate a new Dlkey for the test public key so we can do secret agreement with
    // (test public key, caller private key)
    pkTestKeyPublic = SymCryptDlkeyAllocate( pkCallerKeyPrivate->pDlgroup );
    if( pkTestKeyPublic == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    scError = SymCryptDlkeySetValue(
        NULL, // pbPrivateKey
        0, // cbPrivateKey
        pbKeyBufferPublic,
        cbKeyBufferPublic,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        0,
        pkTestKeyPublic );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    // Allocate buffer for secrets and do secret agreement
    pbSecret1 = SymCryptCallbackAlloc( cbKeyBufferPublic );
    if( pbSecret1 == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    pbSecret2 = SymCryptCallbackAlloc( cbKeyBufferPublic );
    if( pbSecret2 == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    scError = SymCryptDhSecretAgreement(
        pkCallerKeyPrivate,
        pkTestKeyPublic,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_FLAG_KEY_MINIMAL_VALIDATION,
        pbSecret1,
        cbKeyBufferPublic );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    scError = SymCryptDhSecretAgreement(
        pkTestKeyPrivate,
        pkCallerKeyPublic,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_FLAG_KEY_MINIMAL_VALIDATION,
        pbSecret2,
        cbKeyBufferPublic );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    if( memcmp(pbSecret1, pbSecret2, cbKeyBufferPublic) != 0 )
    {
        scError = SYMCRYPT_FIPS_FAILURE;
        goto cleanup;
    }

cleanup:

    if( pbSecret2 != NULL )
    {
        SymCryptCallbackFree( pbSecret2 );
        pbSecret2 = NULL;
    }

    if( pbSecret1 != NULL )
    {
        SymCryptCallbackFree( pbSecret1 );
        pbSecret1 = NULL;
    }

    if( pkTestKeyPublic != NULL )
    {
        SymCryptDlkeyFree( pkTestKeyPublic );
        pkTestKeyPublic = NULL;
    }

    if( pkTestKeyPrivate != NULL )
    {
        SymCryptDlkeyFree( pkTestKeyPrivate );
        pkTestKeyPrivate = NULL;
    }

    if( pkCallerKeyPublic != NULL )
    {
        SymCryptDlkeyFree( pkCallerKeyPublic );
        pkCallerKeyPublic = NULL;
    }

    if( pbKeyBufferPublic != NULL )
    {
        SymCryptCallbackFree( pbKeyBufferPublic );
        pbKeyBufferPublic = NULL;
    }

    return scError;

}

SYMCRYPT_ERROR
SYMCRYPT_CALL
SymCryptEcDhSecretAgreementSelftest(
    _In_ PCSYMCRYPT_ECKEY pkCallerKeyPrivate )
{
    SYMCRYPT_ERROR scError = SYMCRYPT_NO_ERROR;

    PSYMCRYPT_ECKEY pkTestKeyPrivate = NULL;
    PSYMCRYPT_ECKEY pkCallerKeyPublic = NULL;
    PSYMCRYPT_ECKEY pkTestKeyPublic = NULL;

    PBYTE pbKeyBufferPublic = NULL;
    UINT32 cbKeyBufferPublic = 0;

    PBYTE pbSecret1 = NULL;
    PBYTE pbSecret2 = NULL;
    UINT32 cbSecret = 0;

    // Export the caller public key and import it into a new Eckey so we can do secret
    // agreement with (caller public key, test private key)
    cbKeyBufferPublic = SymCryptEckeySizeofPublicKey(
        pkCallerKeyPrivate, SYMCRYPT_ECPOINT_FORMAT_XY );
    pbKeyBufferPublic = SymCryptCallbackAlloc( cbKeyBufferPublic );
    if( pbKeyBufferPublic == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    scError = SymCryptEckeyGetValue(
        pkCallerKeyPrivate,
        NULL,
        0,
        pbKeyBufferPublic,
        cbKeyBufferPublic,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_ECPOINT_FORMAT_XY,
        0 );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    pkCallerKeyPublic = SymCryptEckeyAllocate( pkCallerKeyPrivate->pCurve );
    if( pkCallerKeyPublic == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    scError = SymCryptEckeySetValue(
        NULL,
        0,
        pbKeyBufferPublic,
        cbKeyBufferPublic,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_ECPOINT_FORMAT_XY,
        0,
        pkCallerKeyPublic );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    // Import or generate a test private key. This key must be the same size as the caller key,
    // so if our pre-generated key doesn't match, we have to generate a new one, which is expensive.
    pkTestKeyPrivate = SymCryptEckeyAllocate( pkCallerKeyPrivate->pCurve );
    if( pkTestKeyPrivate == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    scError = SymCryptEckeySetRandom( 0, pkTestKeyPrivate );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    scError = SymCryptEckeyGetValue(
        pkTestKeyPrivate,
        NULL,
        0,
        pbKeyBufferPublic,
        cbKeyBufferPublic,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_ECPOINT_FORMAT_XY,
        0 );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    pkTestKeyPublic = SymCryptEckeyAllocate( pkCallerKeyPrivate->pCurve );
    if( pkTestKeyPublic == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    scError = SymCryptEckeySetValue(
        NULL,
        0,
        pbKeyBufferPublic,
        cbKeyBufferPublic,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_ECPOINT_FORMAT_XY,
        0,
        pkTestKeyPublic );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    cbSecret = SymCryptEcurveSizeofFieldElement( pkCallerKeyPrivate->pCurve );
    pbSecret1 = SymCryptCallbackAlloc( cbSecret );
    if( pbSecret1 == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    pbSecret2 = SymCryptCallbackAlloc( cbSecret );
    if( pbSecret2 == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    scError = SymCryptEcDhSecretAgreement(
        pkCallerKeyPrivate,
        pkTestKeyPublic,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_FLAG_KEY_MINIMAL_VALIDATION,
        pbSecret1,
        cbSecret);
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    scError = SymCryptEcDhSecretAgreement(
        pkTestKeyPrivate,
        pkCallerKeyPublic,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SYMCRYPT_FLAG_KEY_MINIMAL_VALIDATION,
        pbSecret2,
        cbSecret);
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    if( memcmp(pbSecret1, pbSecret2, cbSecret) != 0 )
    {
        scError = SYMCRYPT_FIPS_FAILURE;
        goto cleanup;
    }

cleanup:
    if( pbSecret2 != NULL )
    {
        SymCryptCallbackFree( pbSecret2 );
        pbSecret2 = NULL;
    }

    if( pbSecret1 != NULL )
    {
        SymCryptCallbackFree( pbSecret2 );
        pbSecret1 = NULL;
    }

    if( pkTestKeyPublic != NULL )
    {
        SymCryptEckeyFree( pkTestKeyPublic );
        pkTestKeyPublic = NULL;
    }

    if( pkTestKeyPrivate != NULL )
    {
        SymCryptEckeyFree( pkTestKeyPrivate );
        pkTestKeyPrivate = NULL;
    }

    if( pkCallerKeyPublic != NULL )
    {
        SymCryptEckeyFree( pkCallerKeyPublic );
        pkCallerKeyPublic = NULL;
    }

    if ( pbKeyBufferPublic != NULL )
    {
        SymCryptCallbackFree( pbKeyBufferPublic );
        pbKeyBufferPublic = NULL;
    }

    return scError;
}

SYMCRYPT_ERROR
SYMCRYPT_CALL
SymCryptDsaPairwiseSelftest(
    _In_ PCSYMCRYPT_DLKEY pkCallerKey )
{
    SYMCRYPT_ERROR scError = SYMCRYPT_NO_ERROR;

    BYTE rbHashValue[SYMCRYPT_SHA256_RESULT_SIZE];
    SIZE_T cbHashValue = sizeof(rbHashValue);

    PBYTE pbSignature = NULL;
    SIZE_T cbSignature = 0;

    scError = SymCryptCallbackRandom( rbHashValue, cbHashValue );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    cbSignature = 2 * SymCryptDlkeySizeofPrivateKey( pkCallerKey );
    pbSignature = SymCryptCallbackAlloc( cbSignature );
    if( pbSignature == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    scError = SymCryptDsaSign(
                pkCallerKey,
                rbHashValue,
                cbHashValue,
                SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
                0,
                pbSignature,
                cbSignature );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    scError = SymCryptDsaVerify(
                pkCallerKey,
                rbHashValue,
                cbHashValue,
                pbSignature,
                cbSignature,
                SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
                0 );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

cleanup:

    if( pbSignature != NULL )
    {
        SymCryptCallbackFree( pbSignature );
        pbSignature = NULL;
    }

    return scError;
}

SYMCRYPT_ERROR
SYMCRYPT_CALL
SymCryptEcDsaPairwiseSelftest(
    _In_ PCSYMCRYPT_ECKEY pkCallerKey )
{
    SYMCRYPT_ERROR scError = SYMCRYPT_NO_ERROR;

    BYTE rbHashValue[SYMCRYPT_SHA256_RESULT_SIZE];
    SIZE_T cbHashValue = sizeof(rbHashValue);

    PBYTE pbSignature = NULL;
    SIZE_T cbSignature = 0;

    scError = SymCryptCallbackRandom( rbHashValue, cbHashValue );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    cbSignature = 2 * SymCryptEcurveSizeofFieldElement( pkCallerKey->pCurve );
    pbSignature = SymCryptCallbackAlloc( cbSignature );
    if( pbSignature == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    scError = SymCryptEcDsaSign(
        pkCallerKey,
        rbHashValue,
        cbHashValue,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        0,
        pbSignature,
        cbSignature );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    scError = SymCryptEcDsaVerify(
        pkCallerKey,
        rbHashValue,
        cbHashValue,
        pbSignature,
        cbSignature,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        0 );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

cleanup:

    if(pbSignature != NULL)
    {
        SymCryptCallbackFree( pbSignature );
        pbSignature = NULL;
    }

    return scError;
}

SYMCRYPT_ERROR
SYMCRYPT_CALL
SymCryptRsaPairwiseSelftest(
    _In_ PCSYMCRYPT_RSAKEY pkCallerKey )
{
    SYMCRYPT_ERROR scError = SYMCRYPT_NO_ERROR;

    BYTE rbHashValue[SYMCRYPT_SHA256_RESULT_SIZE] = { 0 };
    SIZE_T cbHashValue = sizeof(rbHashValue);

    BYTE pbSignature = NULL;
    SIZE_T cbSignature = 0;

    scError = SymCryptCallbackRandom( rbHashValue, cbHashValue );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    cbSignature = pkCallerKey->nBitsOfModulus / 8;
    pbSignature = SymCryptCallbackAlloc( cbSignature );
    if( pbSignature == NULL )
    {
        scError = SYMCRYPT_MEMORY_ALLOCATION_FAILURE;
        goto cleanup;
    }

    scError = SymCryptRsaPkcs1Sign(
        pkCallerKey,
        rbHashValue,
        cbHashValue,
        SymCryptSha256OidList,
        SYMCRYPT_SHA256_OID_COUNT,
        0,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        pbSignature,
        cbSignature,
        &cbSignature );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

    scError = SymCryptRsaPkcs1Verify(
        pkCallerKey,
        rbHashValue,
        cbHashValue,
        pbSignature,
        cbSignature,
        SYMCRYPT_NUMBER_FORMAT_MSB_FIRST,
        SymCryptSha256OidList,
        SYMCRYPT_SHA256_OID_COUNT,
        0 );
    if( scError != SYMCRYPT_NO_ERROR )
    {
        goto cleanup;
    }

cleanup:

    if( pbSignature != NULL )
    {
        SymCryptCallbackFree( pbSignature );
        pbSignature = NULL;
    }

    return scError;
}