#ifndef SHA384_H
#define SHA384_H

#include <QString>
#include <vector>
#include <QJsonObject>

class SHA384 {
public:
    static QString hash(const QString& input);
    static const uint64_t K[80];

private:
    static uint64_t rotateRight(uint64_t x, uint64_t n);
    static uint64_t shiftRight(uint64_t x, uint64_t n);
    static uint64_t ch(uint64_t x, uint64_t y, uint64_t z);
    static uint64_t maj(uint64_t x, uint64_t y, uint64_t z);
    static uint64_t sigma0(uint64_t x);
    static uint64_t sigma1(uint64_t x);
    static uint64_t gamma0(uint64_t x);
    static uint64_t gamma1(uint64_t x);
    static void padMessage(std::vector<uint8_t>& message);
    static void processBlock(const uint8_t* block, uint64_t* state);
};

#endif // SHA384_H
