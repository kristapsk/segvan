Bitcoin SegWit vanity address generator
=======================================

This is a simple Bitcoin SegWit [vanity address](https://github.com/bitcoinbook/bitcoinbook/blob/develop/ch04.asciidoc#vanity-addresses) generator.

For building you will need `CMake`, `libbitcoin` and `libsodium`.
```
$ cmake .
$ make VERBOSE=1
```

Usage:
```
$ segvan 3Kids
Found vanity address! 3KidsZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
Private key: PKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPK
```

Then use Bitcoin Core 0.15.x console:
```
> importprivkey PKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPKPK "Vanity test" false
(null)
> getaddressesbyaccount "Vanity test"
[
  "1ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ"
]
> addwitnessaddress 1ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
3KidsZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
```


