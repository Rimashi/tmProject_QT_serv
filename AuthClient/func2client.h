// func2client.h
#ifndef FUNC2CLIENT_H
#define FUNC2CLIENT_H

#include <QObject>
#include <QJsonObject>
#include <QByteArray>
#include "rsa.h"

class Func2Client : public QObject {
    Q_OBJECT
public:
    explicit Func2Client(QObject *parent = nullptr);

    QJsonObject prepareLogin(const QString& login, const QString& password);
    QJsonObject prepareRegister(const QString& surname, const QString& name,
                                const QString& login, const QString& password);
    // Методы для работы с ключами
    void setRsaKeys(const RSA::KeyPair& keys);
    bool handleAesKeyResponse(const QJsonObject &response);
        RSA::KeyPair getRsaKeys() const { return clientKeys; }

    // Шифрование данных
    QJsonObject encryptRequestData(const QJsonObject& request);

private:
    RSA::KeyPair clientKeys;
    QByteArray aesKey;
};

#endif // FUNC2CLIENT_H
