//
// moduletest.cpp
// Test executable for SymCrypt module smoke tests
//
// Copyright (c) Microsoft Corporation. Licensed under the MIT license.
//

#include <stddef.h>
#include <stdio.h>

#include "symcrypt.h"

const UINT32 SymCryptSelftestRsaKeySizeBits = 2048;

_Analysis_noreturn_
VOID
SYMCRYPT_CALL
SymCryptFatal(UINT32 fatalCode)
{
    abort();
}

int
main( int argc, _In_reads_( argc ) char * argv[] )
{
    SYMCRYPT_ERROR scError = SYMCRYPT_NO_ERROR;

    SYMCRYPT_MODULE_INIT();

    if( !(SYMCRYPT_DO_FIPS_SELFTESTS) )
    {
        printf("SYMCRYPT_DO_FIPS_SELFTESTS is false; skipping self-test verification.\n");
    }
    else
    {
        SYMCRYPT_FIPS_ASSERT( (g_SymCryptFipsSelftestsPerformed & SYMCRYPT_SELFTEST_STARTUP) != 0 );

        SymCryptDhSecretAgreementSelftest();
        SymCryptEcDhSecretAgreementSelftest();
        SymCryptDsaSelftest();
        SymCryptEcDsaSelftest();
        SymCryptRsaSelftest();
    }
    
    printf( "Success!\n" );

    return 0;
}