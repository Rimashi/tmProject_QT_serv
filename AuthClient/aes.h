#ifndef AES_H
#define AES_H

#include <QByteArray>
#include <QString>

class AES {
public:
    static QByteArray generateKey();
    static QByteArray encrypt(const QByteArray& data, const QByteArray& key);
    static QByteArray decrypt(const QByteArray& data, const QByteArray& key);

private:
    static const uint8_t sbox[256];
    static const uint8_t invSbox[256];
    static const uint8_t rcon[10];
    static void subBytes(QByteArray& state);
    static void shiftRows(QByteArray& state);
    static void mixColumns(QByteArray& state);
    static void addRoundKey(QByteArray& state, const QByteArray& roundKey);
    static void invSubBytes(QByteArray& state);
    static void invShiftRows(QByteArray& state);
    static void invMixColumns(QByteArray& state);
    static QByteArray expandKey(const QByteArray& key);
    static uint8_t gmul(uint8_t a, uint8_t b); // Умножение в поле Галуа
};

#endif // AES_H
