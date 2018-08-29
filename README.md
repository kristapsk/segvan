Bitcoin SegWit vanity address generator
=======================================

This is a simple Bitcoin SegWit [vanity address](https://github.com/bitcoinbook/bitcoinbook/blob/develop/ch04.asciidoc#vanity-addresses) generator.

Inspired by the vanity address miner for the P2PKH non-SegWit addresses from the book ["Mastering Bitcoin" by Andreas Antonopoulos](https://github.com/bitcoinbook/bitcoinbook/). I put this code under public domain, you can do whatever you want with it.

For building you will need C++11 compatible compiler, `CMake`, `libbitcoin`, `libsodium`, and [`secp256k1`](https://github.com/bitcoin-core/secp256k1) .
```
$ cmake .
$ make VERBOSE=1
```

Usage:
```
Usage: segvan [options] pattern
Options:
        -d      Enable debug output
        -i      Case insensitive matching
        -t num  Number of CPU threads to use (default 1)
```

Example:
```
$ time ./segvan 3Kids
Found vanity address! 3KidsZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
Private key: KPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKP

real    47m28.866s
user    44m58.046s
sys     2m30.354s
```

Then use Bitcoin Core 0.15.x console:
```
> importprivkey KPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKP "Vanity test" false
(null)
> getaddressesbyaccount "Vanity test"
[
  "1ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ"
]
> addwitnessaddress 1ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
3KidsZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
```


