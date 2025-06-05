#ifndef RSA_H
#define RSA_H

#include <string>
#include <vector>
#include <cstdint>

class RSA {
public:
    struct KeyPair {
        std::string publicKey;
        std::string privateKey;
    };

    static KeyPair generateKeys(int bits = 512);
    static std::string encrypt(const std::string& message, const std::string& pubKey);
    static std::string decrypt(const std::string& cipher, const std::string& privKey);

private:
    static bool isPrime(uint64_t n);
    static uint64_t modExp(uint64_t base, uint64_t exp, uint64_t mod);
    static uint64_t randomPrime(int bits);
    static uint64_t gcd(uint64_t a, uint64_t b);
    static uint64_t modInverse(uint64_t a, uint64_t m);
    static std::vector<std::string> split(const std::string& s, char delimiter);
};

#endif 