# cpp-miner
Basic c++ miner. This will not mine actual Bitcoin (it is much too slow), but is useful for testing, generating test fixtures, and learning how Bitcoin mining works at a basic level.

## Installation
```
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install
```

## Usage
The cpp_miner accepts the block header in two ways:
```
Usage: mine <blockHeader 80B> <target 32B> OR <version 4B> <hashPrevBlock 32B> <merkleRoot 32B> <time 4B> <nBits 4B> <nonce 4B> <target 32B>
```
Example:
```
$ src/cpp_miner mine 0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890100000000 0fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
block header: 0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890104000000
nonce: 0x04000000
block hash: 04853701f0da56137ae26bdbb2dd7205d493f7eee4b425da26592f2bd53765bc
```
