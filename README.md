# cpp-miner
Basic c++ miner. This will not mine actual Bitcoin (it is much too slow), but is useful for testing, generating test fixtures, and learning how Bitcoin mining works at a basic level.

## Dependencies
**libsecp256k1**:
```
$ git clone https://github.com/bitcoin-core/secp256k1.git && cd secp256k1
$ ./autogen.sh
$ ./configure
$ sudo make install
```

### macOS
`$ brew install openssl`
libsecp256k1
boost
##$ Linux
libsecp256k1
boost

## Installation
```
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install
```
Note: To build on MacOS, configure using `LDFLAGS="-L/usr/local/opt/openssl/lib" ./configure`

## Usage
```
$ cpp_miner

Usage: $ cpp_miner command [options]
Allowed options:
  --help                      Show this help message
  -v [ --verbose ] [=arg(=1)] Enable verbosity (optionally specify level)
  -t [ --some_flag ]          Use some_flag configuration
  --command arg               Command

Commands
$ cpp_miner mine <blockHeader 80B> OR <version 4B> <hashPrevBlock 32B> <merkleRoot 32B> <time 4B> <nBits 4B> <nonce 4B>
$ cpp_miner genesisgen <pubkey 65B> "<coinbase-message 91B>" value <int> <time 4B> <nBits 4B> <nonce 4B>
```

You can mine a given block header using `$ cpp_miner mine` or you can mine a genesis block from the given genesis block parameters using `$ cpp_miner genesisgen`.

**Mine Example**:
```
$ cpp_miner mine 0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901231dffffff00000000
final nBits -1635801744
target: 000000ffffff0000000000000000000000000000000000000000000000000000
block header: 0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901231dffffff00d55804
nonce: 0x00d55804
block hash: 000000e3100c2c24d5e61224d17f0ff160b14f03f70a51351920dbb1830cbeb8
```

**Genesis Gen Example**:
```
$ cpp_miner genesisgen 04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks" 50 1231006505 486604799 2083236893
block hash: 000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f
block header: 0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a29ab5f49ffff001d1dac2b7c
nonce: 0x1dac2b7c
block hash: 000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f
merkle root: 4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b
```
