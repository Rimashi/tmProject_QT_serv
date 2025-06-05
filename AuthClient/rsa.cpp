#include "rsa.h"
#include <QRandomGenerator>
#include <random>
#include <QDataStream>

// Генерация простого числа
void RSA::generatePrime(mpz_class& result, gmp_randclass& randGen, unsigned int bits) {
    do {
        result = randGen.get_z_bits(bits);
        mpz_setbit(result.get_mpz_t(), bits - 1); // Гарантируем размер
    } while (!mpz_probab_prime_p(result.get_mpz_t(), 30));
}

bool RSA::isPrime(const mpz_class& n, int k) {
    return mpz_probab_prime_p(n.get_mpz_t(), k) > 0;
}

mpz_class RSA::modInverse(const mpz_class& e, const mpz_class& phi) {
    mpz_class d;
    if (mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t()) == 0) {
        qWarning() << "modInverse: обратный элемент не существует";
        return 0;
    }
    return d;
}

// Генерация ключей (оптимизированная)
RSA::KeyPair RSA::generateKeys(unsigned int bits) {
    KeyPair keys;
    mpz_class p, q, n, phi, e(65537), d;

    // Инициализация ГСЧ
    gmp_randclass randGen(gmp_randinit_default);
    std::random_device rd;
    randGen.seed(rd());

    // Генерация простых чисел
    generatePrime(p, randGen, bits / 2);
    do {
        generatePrime(q, randGen, bits / 2);
    } while (q == p);

    n = p * q;
    phi = (p - 1) * (q - 1);

    // Проверка взаимной простоты
    mpz_class gcd;
    mpz_gcd(gcd.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());
    if (gcd != 1) {
        qWarning() << "e=65537 не подходит, генерируем новое e";
        do {
            e = randGen.get_z_bits(bits / 4);
            mpz_setbit(e.get_mpz_t(), 16);
            mpz_gcd(gcd.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());
        } while (gcd != 1);
    }

    d = modInverse(e, phi);

    keys.publicKey = QString::fromStdString(e.get_str()) + ":" + QString::fromStdString(n.get_str());
    keys.privateKey = QString::fromStdString(d.get_str()) + ":" + QString::fromStdString(n.get_str());

    return keys;
}

// Шифрование строки с Base64 кодированием
QString RSA::encrypt(const QString& data, const QString& publicKey) {
    QByteArray encrypted = encryptBytes(data.toUtf8(), publicKey);
    return QString::fromLatin1(encrypted.toBase64());
}

QString RSA::decrypt(const QString& encrypted, const QString& privateKey) {
    QByteArray data = QByteArray::fromBase64(encrypted.toLatin1());
    return QString::fromUtf8(decryptBytes(data, privateKey));
}

// Шифрование байтов с блочной обработкой
QByteArray RSA::encryptBytes(const QByteArray& data, const QString& publicKey) {
    QStringList parts = publicKey.split(':');
    if (parts.size() != 2) return QByteArray();

    mpz_class e(parts[0].toStdString());
    mpz_class n(parts[1].toStdString());
    const size_t block_size = (mpz_sizeinbase(n.get_mpz_t(), 2) / 8 - 1);

    QByteArray result;
    for (int i = 0; i < data.size(); i += block_size) {
        QByteArray block = data.mid(i, block_size);
        mpz_class m;
        mpz_import(m.get_mpz_t(), block.size(), 1, 1, 0, 0, block.constData());

        mpz_class c;
        mpz_powm(c.get_mpz_t(), m.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());

        char* buf = new char[block_size + 16];
        size_t count;
        mpz_export(buf, &count, 1, 1, 0, 0, c.get_mpz_t());
        result.append(buf, static_cast<int>(count));
        delete[] buf;
    }

    return result.toBase64();
}

// Расшифровка байтов
QByteArray RSA::decryptBytes(const QByteArray& encrypted, const QString& privateKey) {
    QByteArray data = QByteArray::fromBase64(encrypted);
    QStringList parts = privateKey.split(':');
    if (parts.size() != 2) return QByteArray();

    mpz_class d(parts[0].toStdString());
    mpz_class n(parts[1].toStdString());
    const size_t block_size = (mpz_sizeinbase(n.get_mpz_t(), 2) / 8);

    QByteArray result;
    for (int i = 0; i < data.size(); i += block_size) {
        QByteArray block = data.mid(i, block_size);
        mpz_class c;
        mpz_import(c.get_mpz_t(), block.size(), 1, 1, 0, 0, block.constData());

        mpz_class m;
        mpz_powm(m.get_mpz_t(), c.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());

        char* buf = new char[block_size + 16];
        size_t count;
        mpz_export(buf, &count, 1, 1, 0, 0, m.get_mpz_t());
        result.append(buf, static_cast<int>(count));
        delete[] buf;
    }

    return result;
}
