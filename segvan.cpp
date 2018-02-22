#include <cstdlib>
#include <iostream>
#include <sodium.h>
#include <bitcoin/bitcoin.hpp>

using namespace bc;
using namespace libbitcoin::chain;
using namespace libbitcoin::machine;
using namespace libbitcoin::wallet;

void usage (void)
{
    std::cout << "Usage: segvan pattern" << std::endl;
}

ec_secret random_secret (void)
{
    char randbytes[ec_secret_size];
    ec_secret secret;
    randombytes_buf(randbytes, ec_secret_size);
    std::copy(std::begin(randbytes), std::end(randbytes), std::begin(secret));
    return secret;
}

operation::list witness_program (const ec_compressed& pubkey)
{
    short_hash keyhash = bitcoin_short_hash(pubkey);
    return {
        operation(opcode(0)),
        operation(to_chunk(keyhash))
    };
}

std::string p2sh_address (const ec_secret& secret, bool testnet)
{
    ec_compressed pubkey;
    if (secret_to_public(pubkey, secret)) {
        return payment_address(
            script(witness_program(pubkey)),
            (testnet ? payment_address::testnet_p2sh : payment_address::mainnet_p2sh)
        ).encoded();
    }
    return "";
}

int main (int argc, char** argv)
{
    if (argc != 2) {
        usage();
        return EXIT_FAILURE;
    }
    std::string pattern = argv[1];
    bool testnet;
    if (pattern[0] == '2') {
        testnet = true;
    }
    else if (pattern[0] == '3') {
        testnet = false;
    }
    else {
        std::cerr << "Invalid address patern " << pattern << std::endl;
        return EXIT_FAILURE;
    }

    while (true)
    {
        ec_secret secret = random_secret();
        std::string address = p2sh_address(secret, testnet);
        if (!address.empty()) {
            //std::cout << "Trying " << address << std::endl;
            if (address.compare(0, pattern.size(), pattern) == 0) {
                std::cout << "Found vanity address! " << address << std::endl;
                ec_private privkey(secret, 0x8000, true);
                std::cout << "Private key: " << privkey.encoded() << std::endl;
                return EXIT_SUCCESS;
            }
        }
    }

    return EXIT_FAILURE;
}
