#include "func2client.h"
#include "singleton_client.h"
#include <sstream>
#include <iomanip>
#include <cstring>
#include <vector>

#define ROTR(x, n) (((x) >> (n)) | ((x) << (64 - (n))))
#define SHR(x, n) ((x) >> (n))
uint64_t Ch(uint64_t x, uint64_t y, uint64_t z) { return (x & y) ^ (~x & z); }
uint64_t Maj(uint64_t x, uint64_t y, uint64_t z) { return (x & y) ^ (x & z) ^ (y & z); }
uint64_t Sigma0(uint64_t x) { return ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39); }
uint64_t Sigma1(uint64_t x) { return ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41); }
uint64_t sigma0(uint64_t x) { return ROTR(x, 1) ^ ROTR(x, 8) ^ SHR(x, 7); }
uint64_t sigma1(uint64_t x) { return ROTR(x, 19) ^ ROTR(x, 61) ^ SHR(x, 6); }

const uint64_t K[80] = { /* ... как у тебя выше ... */ };
const uint64_t H_init[8] = { /* ... как у тебя выше ... */ };

std::vector<uint8_t> pad_message(const std::string& message) {
    std::vector<uint8_t> padded(message.begin(), message.end());
    padded.push_back(0x80);
    while ((padded.size() + 16) % 128 != 0) padded.push_back(0x00);
    uint64_t bit_len = message.size() * 8;
    for (int i = 0; i < 8; ++i) padded.push_back(0);
    for (int i = 7; i >= 0; --i) padded.push_back((bit_len >> (i * 8)) & 0xff);
    return padded;
}

void process_block(const uint8_t* block, uint64_t* H) {
    uint64_t W[80];
    for (int t = 0; t < 16; ++t) {
        W[t] = 0;
        for (int i = 0; i < 8; ++i)
            W[t] = (W[t] << 8) | block[t * 8 + i];
    }
    for (int t = 16; t < 80; ++t) {
        W[t] = sigma1(W[t - 2]) + W[t - 7] + sigma0(W[t - 15]) + W[t - 16];
    }

    uint64_t a = H[0], b = H[1], c = H[2], d = H[3];
    uint64_t e = H[4], f = H[5], g = H[6], h = H[7];

    for (int t = 0; t < 80; ++t) {
        uint64_t T1 = h + Sigma1(e) + Ch(e, f, g) + K[t] + W[t];
        uint64_t T2 = Sigma0(a) + Maj(a, b, c);
        h = g; g = f; f = e; e = d + T1;
        d = c; c = b; b = a; a = T1 + T2;
    }

    H[0] += a; H[1] += b; H[2] += c; H[3] += d;
    H[4] += e; H[5] += f; H[6] += g; H[7] += h;
}

std::string sha384(const std::string& message) {
    uint64_t H[8];
    std::memcpy(H, H_init, sizeof(H));
    std::vector<uint8_t> padded = pad_message(message);
    for (size_t i = 0; i < padded.size(); i += 128) {
        process_block(&padded[i], H);
    }
    std::ostringstream oss;
    for (int i = 0; i < 6; ++i) {
        oss << std::hex << std::setw(16) << std::setfill('0') << std::nouppercase << H[i];
    }
    return oss.str();
}

QString func2client::registerUser(const QString& username, const QString& password) {
    QJsonObject obj;
    obj["action"] = "register";
    obj["username"] = username;
    obj["password"] = QString::fromStdString(sha384(password.toStdString()));

    QJsonArray request;
    request.append(obj);

    QString response = Singleton_client::getInstance()->send_request(request);
    return response;
}

QString func2client::loginUser(const QString& username, const QString& password) {
    QJsonObject obj;
    obj["action"] = "login";
    obj["username"] = username;
    obj["password"] = QString::fromStdString(sha384(password.toStdString()));

    QJsonArray request;
    request.append(obj);

    QString response = Singleton_client::getInstance()->send_request(request);
    return response;
}

QString func2client::getStatistics() {
    QJsonObject obj;
    obj["action"] = "get_statistics";

    QJsonArray request;
    request.append(obj);

    QString response = Singleton_client::getInstance()->send_request(request);
    return response;
}
