# Introduction
SymCrypt is the core cryptographic function library currently used by Windows.

## History
The library was started in late 2006 with the first sources committed in Feb 2007.
Initially the goal was limited to implement symmetric cryptographic operations, hence the name.
Starting with Windows 8, it has been the primary crypto library for symmetric algorithms.

In 2015 we started the work of adding asymmetric algorithms to SymCrypt. Since the 1703 release of Windows 10,
SymCrypt has been the primary crypto library for all algorithms in Windows.

## Goals
Like any engineering project, SymCrypt is a compromise between conflicting requirements:
- Provide safe implementations of the cryptographic algorithms needed by Microsoft products.
- Run on all CPU architectures supported by Windows.
- Good performance.
- Minimize maintenance cost.
- Support FIPS 140-2 certification of products using SymCrypt.
- Provide high assurance in the proper functionality of the library.

# Build and Test
SymCrypt can be compiled with CMake >= 3.13.0 and Visual Studio 2019 (with Windows 10 SDK version 18362) on Windows
or gcc 7.4.0 or clang 10.0.0 on Linux. Note that CMake ships with Visual Studio 2019.

Python3 is also required for translation of SymCryptAsm, and for building the SymCrypt module with integrity check.
The integrity check additionally requires pip and pyelftools: `pip3 install -r ./scripts/requirements.txt`


1. Optionally use CMake from Visual Studio `$env:PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\;${env:PATH}"`
2. For Microsoft employees building the library internally, to include msbignum and RSA32 implementation benchmarks in the unit tests:
    1. `git submodule update --init`
    2. In step 4 below, add the additional cmake argument `-DSYMCRYPT_INTERNAL_BUILD=1`
3. `mkdir bin; cd bin`
4. Configure CMake compilation:
    * For 32-bit Windows targets: `cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake-toolchain/WindowsUserMode-X86.cmake -A Win32`
    * For 64-bit Windows targets: `cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake-toolchain/WindowsUserMode-AMD64.cmake`
    * For 64-bit Linux targets: `cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake-toolchain/LinuxUserMode-AMD64.cmake`
    * For no CPU optimizations: `cmake ..`
    * Optionally, for a release build, specify `-DCMAKE_BUILD_TYPE=Release`
5. `cmake --build .`
    * Optionally, for a release build on Windows, specify `--config Release`
    * Optionally specify `-jN` where N is the number of processes you wish to spawn for the build

If compilation succeeds, the output will be put in the `exe` subdirectory relative to where compilation occurred
(i.e. `bin/exe` if you followed the instructions above).

The SymCrypt unit test is in the `unittest` directory. It runs extensive functional tests on the SymCrypt
library. On Windows it also compares results against on other implementations such as the Windows APIs CNG
and CAPI, and the older crypto libraries rsa32 and msbignum, if they are available. It also provides
detailed performance information.

# Security Bugs
If you believe you have found a problem that affects the security of this code, please do **NOT** create an issue
or pull request, but instead email your comments to secure@microsoft.com.

# Contribute
We love to receive comments and suggestions. Unfortunately we cannot accept external code contributions at this time.
Cryptographic code is considered highly sensitive by many of our large customers.
We have some very big customers who put great value in the assurance of the crypto code used in their organization.
By restricting the coding to a handful of employees we can greatly reduce the (perceived) risk of malicious contributions.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.


