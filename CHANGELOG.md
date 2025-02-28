# Version 102.0

- Breaking change to Asymmetric key generation and import handling, sanitizing flags required for FIPS
- Trim symbols exposed in SymCrypt module to those specified in symcrypt.h
- Introduce logic enabling FIPS per-key tests to be deferred to before first use, rather than at generation time

# Version 101.3

- Fix for OpenEnclave binary to workaround clang bug
- Fix SymCryptRsaPssVerify to return SYMCRYPT_SIGNATURE_VERIFICATION_FAILURE for invalid signatures, rather than SYMCRYPT_INVALID_ARGUMENT
- Fix for SymCryptShortWeierstrassAddSideChannelUnsafe which is only use in ECDSA verification on SW curves
  - An owner of a private ECC key could create an invalid signature that SymCrypt would verify, leaking their private key in the process

# Version 101.2

- Added key pairwise consistency tests for RSA, DSA, DH, ECDSA and ECDH key generation, per FIPS 140-3 requirements
- Added Session APIs for AES-GCM
  - For Encryption, this enables multithreaded IV generation within the SymCrypt FIPS boundary
  - For Decryption, this enables multithreaded replay protection detecting reuse of IVs in received messages

# Version 101.1

- Support for Group 20 in SAE method

# Version 101.0

- Support stable ABI: change SYMCRYPT_ERROR definition so that error values no longer change with version changes
- Support proper shared library versioning in CMake build scripts
- Support for FIPS integrity verification on ARM64
- Additional CMake build system changes

# Version 100.21

- Fix bug in SymCryptDlkeySizeofPrivateKey
- Add SymCryptDlkeySetPrivateKeyLength API
- Add SymCryptHkdfExtractPrk API
- Add SP800-108 self tests for HMAC-SHA512
- Use _mmXXX_storeu_siXXX intrinsics in AES-XTS w/VAES
- Add macros for volatile memory access to avoid MSVC C4746 warning in unit tests