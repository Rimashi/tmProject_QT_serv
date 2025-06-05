#include <QtTest>
#include <string>
#include <iostream>
#include "C:\QTCreator\projects\RSA\rsa.h"

class RsaTest : public QObject
{
    Q_OBJECT

private slots:
    void test_key_generation();
    void test_encryption_decryption();
    void test_empty_string();
    void test_large_data();
};

void RsaTest::test_key_generation()
{
    RSA rsa;
    RSA::KeyPair keys = rsa.generateKeys(1024);

    qDebug("Generated public key: %s", keys.publicKey.toStdString().c_str());
    qDebug("Generated private key: %s", keys.privateKey.toStdString().c_str());

    QVERIFY2(!keys.publicKey.isEmpty(), "Public key should not be empty");
    QVERIFY2(!keys.privateKey.isEmpty(), "Private key should not be empty");
    QVERIFY2(keys.publicKey != keys.privateKey, "Public and private keys should be different");
}

void RsaTest::test_encryption_decryption()
{
    RSA rsa;
    RSA::KeyPair keys = rsa.generateKeys(1024);
    QString original = "Test message for RSA encryption";

    QString encrypted = rsa.encrypt(original, keys.publicKey);
    QString decrypted = rsa.decrypt(encrypted, keys.privateKey);

    qDebug("Original: %s", original.toStdString().c_str());
    qDebug("Encrypted: %s", encrypted.toStdString().c_str());
    qDebug("Decrypted: %s", decrypted.toStdString().c_str());

    QVERIFY2(original == decrypted, "Decrypted message should match original");
    QVERIFY2(original != encrypted, "Encrypted message should differ from original");
}

void RsaTest::test_empty_string()
{
    RSA rsa;
    RSA::KeyPair keys = rsa.generateKeys(1024);
    QString original = "";

    QString encrypted = rsa.encrypt(original, keys.publicKey);
    QString decrypted = rsa.decrypt(encrypted, keys.privateKey);

    QVERIFY2(original == decrypted, "Empty string should encrypt/decrypt correctly");
}

void RsaTest::test_large_data()
{
    RSA rsa;
    RSA::KeyPair keys = rsa.generateKeys(2048); // Используем больший ключ для больших данных
    QString original(1024, 'A'); // Строка из 1024 символов 'A'

    QString encrypted = rsa.encrypt(original, keys.publicKey);
    QString decrypted = rsa.decrypt(encrypted, keys.privateKey);

    QVERIFY2(original == decrypted, "Large data should encrypt/decrypt correctly");
}

int main(int argc, char *argv[])
{
    RsaTest tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "main.moc"
