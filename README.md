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
Usage: mine <blockHeader 80B> OR <version 4B> <hashPrevBlock 32B> <merkleRoot 32B> <time 4B> <nBits 4B> <nonce 4B>
```
It will mine until it finds a block hash that is less than the target determined by the nBits field in the header
provided

**Example**:
```
$ src/cpp_miner mine 0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901231dffffff00000000
final nBits -1635801744
target: 000000ffffff0000000000000000000000000000000000000000000000000000
block header: 0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901231dffffff00d55804
nonce: 0x00d55804
block hash: 000000e3100c2c24d5e61224d17f0ff160b14f03f70a51351920dbb1830cbeb8
```
