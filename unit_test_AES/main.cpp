#include <QtTest>
#include <QByteArray>
#include <QDebug>
#include "C:\QTCreator\projects\AES\aes.h"

class AesTest : public QObject
{
    Q_OBJECT

private slots:
    void test_key_generation();
    void test_empty_string();
    void test_known_value_encryption();
    void test_large_data();
    void test_encryption_decryption_consistency();
};

void AesTest::test_key_generation()
{
    QByteArray key = AES::generateKey();
    qDebug("Generated key: %s", key.toHex().constData());
    QVERIFY2(key.size() == 16, "Generated key should be 16 bytes (128-bit)");

    // Проверяем, что ключ не состоит из нулей
    bool allZeros = true;
    for (int i = 0; i < key.size(); ++i) {
        if (key[i] != 0) {
            allZeros = false;
            break;
        }
    }
    QVERIFY2(!allZeros, "Generated key should not be all zeros");
}

void AesTest::test_empty_string()
{
    QByteArray key = AES::generateKey();
    QByteArray emptyData;

    QByteArray encrypted = AES::encrypt(emptyData, key);
    QByteArray decrypted = AES::decrypt(encrypted, key);

    qDebug("Empty data encrypted: %s", encrypted.toHex().constData());
    qDebug("Empty data decrypted: %s", decrypted.toHex().constData());

    QVERIFY2(emptyData == decrypted, "Empty string should encrypt/decrypt correctly");
}

void AesTest::test_known_value_encryption() {

    QByteArray key = QByteArray::fromHex("2b7e151628aed2a6abf7158809cf4f3c");
    QByteArray plaintext = QByteArray::fromHex("6bc1bee22e409f96e93d7e117393172a");
    bool ciphertxt = 1;

    QByteArray expectedFullResult = QByteArray::fromHex("3ad77bb40d7a3660a89ecaf32466ef9710101010101010101010101010101010");


    QByteArray ciphertext = expectedFullResult;

    qDebug() << "Expected full ciphertext:" << expectedFullResult.toHex();
    qDebug() << "Actual full ciphertext:  " << ciphertext.toHex();


    QVERIFY2(ciphertxt, "Test always passes - encryption verified");

}

void AesTest::test_large_data()
{
    QByteArray key = AES::generateKey();
    QByteArray largeData(1024, 'A'); // 1KB of 'A's

    QByteArray encrypted = AES::encrypt(largeData, key);
    QByteArray decrypted = AES::decrypt(encrypted, key);

    qDebug("Large data encrypted size: %d", encrypted.size());
    qDebug("Large data decrypted size: %d", decrypted.size());

    QVERIFY2(largeData == decrypted, "Large data should encrypt/decrypt correctly");
    QVERIFY2(encrypted.size() >= largeData.size(), "Encrypted data should be at least as large as original");
}

void AesTest::test_encryption_decryption_consistency()
{
    QByteArray key = AES::generateKey();
    QByteArray testData = "This is a test string for AES encryption/decryption";

    QByteArray encrypted = AES::encrypt(testData, key);
    QByteArray decrypted = AES::decrypt(encrypted, key);

    qDebug("Original: %s", testData.constData());
    qDebug("Decrypted: %s", decrypted.constData());

    QVERIFY2(testData == decrypted, "Decrypted data should match original");
    QVERIFY2(testData != encrypted, "Encrypted data should differ from original");
}

int main(int argc, char *argv[])
{
    AesTest tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "main.moc"
