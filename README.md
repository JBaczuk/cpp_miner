# cpp-miner
Basic c++ miner. This will not mine actual Bitcoin (it is much too slow), but is useful for testing, generating test fixtures, and learning how Bitcoin mining works at a basic level.

## Dependencies
- openssl
- boost

### Mac
Note: Currently this does not build yet on mac.
```
$ brew install openssl
$ brew install boost
```

### Ubuntu
```
$ sudo apt-get install libboost-all-dev
$ sudo apt-get install openssl
```

## Installation
Option 1:
Build from source
```
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install # Optional
```
Option 2:
Download the latest release from https://github.com/utah-bitcoin-community/cpp_miner/releases/ and either copy to a system directory like `/usr/local/bin` or add the folder to your `$PATH`.

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
