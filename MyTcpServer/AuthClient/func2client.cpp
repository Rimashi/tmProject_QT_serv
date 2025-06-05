// func2client.cpp
#include "func2client.h"
#include <QDebug>
#include "rsa.h"
#include "aes.h"
#include "sha384.h"
#include <QJsonDocument>

Func2Client::Func2Client(QObject *parent) : QObject(parent) {
    // Инициализация ключей при создании
    clientKeys = RSA::generateKeys(512);
    qDebug() << "Generated client RSA keys in Func2Client";
    qDebug() << "Public key:" << clientKeys.publicKey;
    qDebug() << "Private key:" << clientKeys.privateKey;

    // Инициализируем AES ключ как пустой
    aesKey = QByteArray();
}

void Func2Client::setRsaKeys(const RSA::KeyPair& keys) {
    clientKeys = keys;
}

QJsonObject Func2Client::prepareLogin(const QString& login, const QString& password) {
    QJsonObject data;
    data["login"] = login;
    data["password"] = SHA384::hash(password);

    QJsonObject request;
    request["action"] = "login";

    if (!aesKey.isEmpty()) {
        // Создаем JSON-документ из объекта
        QJsonDocument doc(data);
        QByteArray dataBytes = doc.toJson(QJsonDocument::Compact);
        QByteArray encryptedData = AES::encrypt(dataBytes, aesKey);

        request["data"] = QString(encryptedData.toBase64());
    }

    return request;
}

QJsonObject Func2Client::prepareRegister(const QString& surname, const QString& name,
                                         const QString& login, const QString& password) {
    QJsonObject data;
    data["surname"] = surname;
    data["name"] = name;
    data["login"] = login;
    data["password"] = SHA384::hash(password);

    QJsonObject request;
    request["action"] = "register";

    if (!aesKey.isEmpty()) {
        // Преобразуем данные в JSON строку и шифруем
        QJsonDocument doc(data);
        QByteArray dataBytes = doc.toJson(QJsonDocument::Compact);
        QByteArray encryptedData = AES::encrypt(dataBytes, aesKey);

        request["data"] = QString(encryptedData.toBase64());
    }

    return request;
}

bool Func2Client::handleAesKeyResponse(const QJsonObject &response) {

    QJsonObject data = response;
    if (!data.contains("aes_key")) {
        qWarning() << "AES key response missing aes_key field";
        return false;
    }

    QString base64Encrypted = data["aes_key"].toString();
    QByteArray encryptedAes = QByteArray::fromBase64(base64Encrypted.toUtf8());

    if (encryptedAes.isEmpty()) {
        qWarning() << "Failed to decode base64 AES key";
        return false;
    }

    aesKey = RSA::decryptBytes(encryptedAes, clientKeys.privateKey);

    if (aesKey.isEmpty()) {
        qWarning() << "Failed to decrypt AES key";
        return false;
    }

    qDebug() << "AES key set in Func2Client:" << aesKey.toHex();
    return true;
}

QJsonObject Func2Client::encryptRequestData(const QJsonObject& request) {
    if (aesKey.isEmpty()) {
        qWarning() << "Cannot encrypt data: AES key not set";
        return request;
    }

    // Шифруем только поле "data"
    QJsonObject data = request["data"].toObject();
    QJsonDocument doc(data);
    QByteArray dataBytes = doc.toJson(QJsonDocument::Compact);
    QByteArray encryptedData = AES::encrypt(dataBytes, aesKey);

    // Создаем новый запрос с зашифрованными данными
    QJsonObject encryptedRequest;
    encryptedRequest["action"] = request["action"];
    encryptedRequest["data"] = QString(encryptedData.toBase64());

    return encryptedRequest;
}
