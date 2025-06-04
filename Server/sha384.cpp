#include "sha384.h"
#include <QByteArray>
#include <QString>
#include <iomanip>
#include <sstream>
#include <QDebug>
#include <cstdint>

const uint64_t SHA384::K[80] = {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
    0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
    0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
    0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
    0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
    0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
    0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
    0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
    0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
    0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
    0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
    0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
    0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

uint64_t SHA384::rotateRight(uint64_t x, uint64_t n) {
    return (x >> n) | (x << (64 - n));
}

uint64_t SHA384::shiftRight(uint64_t x, uint64_t n) {
    return x >> n;
}

uint64_t SHA384::ch(uint64_t x, uint64_t y, uint64_t z) {
    return (x & y) ^ (~x & z);
}

uint64_t SHA384::maj(uint64_t x, uint64_t y, uint64_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

uint64_t SHA384::sigma0(uint64_t x) {
    return rotateRight(x, 28) ^ rotateRight(x, 34) ^ rotateRight(x, 39);
}

uint64_t SHA384::sigma1(uint64_t x) {
    return rotateRight(x, 14) ^ rotateRight(x, 18) ^ rotateRight(x, 41);
}

uint64_t SHA384::gamma0(uint64_t x) {
    return rotateRight(x, 1) ^ rotateRight(x, 8) ^ shiftRight(x, 7);
}

uint64_t SHA384::gamma1(uint64_t x) {
    return rotateRight(x, 19) ^ rotateRight(x, 61) ^ shiftRight(x, 6);
}

void SHA384::padMessage(std::vector<uint8_t>& message) {
    qDebug() << "SHA384::padMessage: Начало добавления PAD, исходный размер:" << message.size();
    uint64_t originalBitLength = message.size() * 8;

    // Добавляем 0x80
    message.push_back(0x80);

    // Вычисляем, сколько байт нужно добавить до кратности 128, с учётом 16 байт для длины
    size_t currentSize = message.size();
    size_t paddingBytes = (128 - (currentSize % 128) - 16) % 128;
    if (currentSize + paddingBytes + 16 > message.max_size()) {
        qCritical() << "SHA384::padMessage: Слишком большой размер для вектора";
        throw std::length_error("Vector size exceeds max_size");
    }

    // Добавляем нули
    message.insert(message.end(), paddingBytes, 0x00);
    qDebug() << "SHA384::padMessage: Добавлено" << paddingBytes << "байт padding'а";

    // Добавляем длину сообщения (128 бит = 16 байт, big-endian)
    for (int i = 15; i >= 0; --i) {
        message.push_back(static_cast<uint8_t>((originalBitLength >> (i * 8)) & 0xFF));
    }

    qDebug() << "SHA384::padMessage: Подстановка завершена, итоговый размер:" << message.size();
    if (message.size() % 128 != 0) {
        qCritical() << "SHA384::padMessage: Итоговый размер не кратен 128 байтам!";
    }
}

void SHA384::processBlock(const uint8_t* block, uint64_t* state) {
    uint64_t W[80];
    for (int t = 0; t < 16; ++t) {
        W[t] = ((uint64_t)block[t * 8] << 56) |
               ((uint64_t)block[t * 8 + 1] << 48) |
               ((uint64_t)block[t * 8 + 2] << 40) |
               ((uint64_t)block[t * 8 + 3] << 32) |
               ((uint64_t)block[t * 8 + 4] << 24) |
               ((uint64_t)block[t * 8 + 5] << 16) |
               ((uint64_t)block[t * 8 + 6] << 8) |
               ((uint64_t)block[t * 8 + 7]);
    }
    for (int t = 16; t < 80; ++t) {
        W[t] = gamma1(W[t - 2]) + W[t - 7] + gamma0(W[t - 15]) + W[t - 16];
    }

    uint64_t a = state[0], b = state[1], c = state[2], d = state[3];
    uint64_t e = state[4], f = state[5], g = state[6], h = state[7];

    for (int t = 0; t < 80; ++t) {
        uint64_t T1 = h + sigma1(e) + ch(e, f, g) + K[t] + W[t];
        uint64_t T2 = sigma0(a) + maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }

    state[0] += a; state[1] += b; state[2] += c; state[3] += d;
    state[4] += e; state[5] += f; state[6] += g; state[7] += h;
}

QString SHA384::hash(const QString& input) {
    qDebug() << "SHA384::hash: Хеширование строки:" << input;
    QByteArray byteArray = input.toUtf8();
    std::vector<uint8_t> message(byteArray.begin(), byteArray.end());

    padMessage(message);

    uint64_t state[8] = {
        0xcbbb9d5dc1059ed8, 0x629a292a367cd507, 0x9159015a3070dd17, 0x152fecd8f70e5939,
        0x67332667ffc00b31, 0x8eb44a8768581511, 0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4
    };

    for (size_t i = 0; i < message.size(); i += 128) {
        processBlock(message.data() + i, state);
    }

    std::stringstream ss;
    for (int i = 0; i < 6; ++i) {
        ss << std::hex << std::setw(16) << std::setfill('0') << state[i];
    }
    return QString::fromStdString(ss.str());
}
