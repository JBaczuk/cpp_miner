# cpp-miner
Basic c++ miner. This will not mine actual Bitcoin (it is much too slow), but is useful for testing, generating test fixtures, and learning how Bitcoin mining works at a basic level.

## Installation
```
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install # Optional
```

## Usage
The cpp_miner accepts the block header in two ways:
```
$ mine <blockHeader 80B> <target 32B>
```
OR 
```
$ mine <version 4B> <hashPrevBlock 32B> <merkleRoot 32B> <time 4B> <nBits 4B> <nonce 4B> <target 32B>
```

Example:
```
$ cpp_miner mine 0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890100000000 000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
block header: 01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901007227b6
nonce: 0x007227b6
block hash: 00000036f82f676bb8ee663aa1a43419bd394b60d752ff9c851129ee25c570c9
```
