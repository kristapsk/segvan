#include <cstdlib>
#include <iostream>
#include <mutex>
#include <regex>
#include <thread>
#include <unistd.h>

#include <sodium.h>
#include <bitcoin/bitcoin.hpp>

using namespace bc;
using namespace libbitcoin::chain;
using namespace libbitcoin::machine;
using namespace libbitcoin::wallet;

std::regex mainnet_p2sh_pattern("^3[a-km-zA-HJ-NP-Z1-9]{1,34}$");
std::regex testnet_p2sh_pattern("^2[a-km-zA-HJ-NP-Z1-9]{1,34}$");

bool debug_output = false;

std::string pattern;
bool testnet;

std::mutex mtx;

void usage (void)
{
    std::cout <<
        "Usage: segvan [options] pattern\n"
        "Options:\n"
        "\t-d\t\tEnable debug output\n"
        "\t-t num\tNumber of CPU threads to use (default 1)" << std::endl;
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

void vanity_thread (int tid)
{
    while (true)
    {
        ec_secret secret = random_secret();
        std::string address = p2sh_address(secret, testnet);
        if (!address.empty()) {
            if (debug_output) {
                mtx.lock();
                std::cout << "[Thread " << tid << "] Trying " << address << std::endl;
                mtx.unlock();
            }
            if (address.compare(0, pattern.size(), pattern) == 0) {
                mtx.lock();
                std::cout << "Found vanity address! " << address;
                if (testnet) {
                    std::cout << " (testnet)";
                }
                std::cout << std::endl;
                ec_private privkey(secret, 0x8000, true);
                std::cout << "Private key: " << privkey.encoded() << std::endl;
                exit(EXIT_SUCCESS);
            }
        }
        else {
            mtx.lock();
            std::cerr << "Error creating address from secret!" << std::endl;
            abort();
        }
    }
}

int main (int argc, char** argv)
{
    int num_threads = 1;

    int c;
    while ((c = getopt(argc, argv, "dt:")) != -1) {
        switch (c) {
            case 'd':
                debug_output = true;
                break;
            case 't':
                num_threads = atoi(optarg);
                break;
            default:
                abort();
        }
    }

    if (optind == argc) {
        usage();
        return EXIT_FAILURE;
    }

    pattern = argv[optind];

    if (std::regex_match(pattern, mainnet_p2sh_pattern)) {
        testnet = false;
    }
    else if (std::regex_match(pattern, testnet_p2sh_pattern)) {
        testnet = true;
    }
    else {
        std::cerr << "Invalid address patern " << pattern << std::endl;
        usage();
        abort();
    }

    if (debug_output) {
        std::cout << "Searching for address beginning with " << pattern;
        if (testnet) {
            std::cout << " (testnet)";
        }
        std::cout << " using " << num_threads << " CPU thread";
        if (num_threads > 1) {
            std::cout << "s";
        }
        std::cout << std::endl;
    }

    if (num_threads <= 1) {
        vanity_thread(1);
    }
    else {
        std::thread t[num_threads];
        for (int i = 0; i < num_threads; ++i) {
            t[i] = std::thread(vanity_thread, i);
        }
        for (int i = 0; i < num_threads; ++i) {
            t[i].join();
        }
    }

    return EXIT_FAILURE;
}
