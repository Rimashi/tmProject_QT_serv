#ifndef RSA_H
#define RSA_H

#include <QString>
#include <QByteArray>
#include <gmpxx.h>
#include <QDebug>

class RSA {
public:
    struct KeyPair {
        QString publicKey;
        QString privateKey;
    };

    static KeyPair generateKeys(unsigned int bits);
    static QString encrypt(const QString& data, const QString& publicKey);
    static QString decrypt(const QString& encrypted, const QString& privateKey);
    static QByteArray encryptBytes(const QByteArray& data, const QString& publicKey);
    static QByteArray decryptBytes(const QByteArray& encrypted, const QString& privateKey);

private:
    static bool isPrime(const mpz_class& n, int k = 25);
    static mpz_class modInverse(const mpz_class& e, const mpz_class& phi);
    static void generatePrime(mpz_class& result, gmp_randclass& randGen, unsigned int bits);
};

#endif // RSA_H
