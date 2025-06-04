// func2serv.cpp
#include "func2serv.h"
#include "server_api.h"
#include "rsa.h"
#include "aes.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Func2Serv::Func2Serv(Server_api* serverApi, QObject *parent) : QObject(parent), serverApi(serverApi) {
    if (!db.open("test.sqlite")) {
        qCritical() << "Func2Serv: Не удалось подключиться к базе данных!";
        // Можно кинуть исключение или обработать ошибку
    }
    db.initializeAdmin();
}

QJsonObject Func2Serv::handleRegistration(const QJsonObject &data) {
    QJsonObject response;
    response["action"] = "register_result";

    QString surname = data["surname"].toString();
    QString name = data["name"].toString();
    QString login = data["login"].toString();
    QString password = data["password"].toString();

    qDebug() << surname << "-" << name << "-" << login;

    if (db.userExists(login)) {
        response["success"] = false;
        response["error"] = "Пользователь с таким логином уже существует";
        return response;
    }

    // QString passwordHash = SHA384::hash(password); // хешируем пароль
    QString passwordHash = password;

    bool registered = db.registerUser(login, passwordHash, name, surname);
    if (registered) {
        response["success"] = true;
    } else {
        response["success"] = false;
        response["error"] = "Ошибка регистрации";
    }

    return response;
}

QJsonObject Func2Serv::handleLogin(const QJsonObject &data) {
    QJsonObject response;
    response["action"] = "login_result";

    QString login = data["login"].toString();
    QString password = data["password"].toString();

    // QString passwordHash = SHA384::hash(password); // хешируем пароль
    QString passwordHash = password;

    QJsonObject userData;
    if (db.validateUser(login, passwordHash, userData)) {
        response["success"] = true;
        response["user_id"] = userData["id"];
        response["surname"] = userData["lastName"];
        response["name"] = userData["firstName"];
        response["role"] = userData["role"];
    } else {
        response["success"] = false;
        response["error"] = "Неверный логин или пароль";
    }
    return response;
}

QJsonObject Func2Serv::handlePublicKey(const QJsonObject &data, QTcpSocket *socket){
    QJsonObject response;
    QJsonObject responseData;

    if (!data.contains("public_key")) {
        response["action"] = "error";
        response["message"] = "Missing public key";
        return response;
    }

    QString clientPublicKey = data["public_key"].toString();
    qDebug() << "Client public key:" << clientPublicKey;

    if (clientPublicKey.isEmpty()) {
        qCritical() << "Received empty public key!";
        response["action"] = "error";
        response["message"] = "Empty public key received";
        return response;
    }

    // Генерация AES ключа
    QByteArray aesKey = AES::generateKey();  // 128-битный ключ
    qDebug() << "Generated AES key:" << aesKey.toHex();

    // Сохраняем AES ключ для этого клиента
    serverApi->addAesKey(socket, aesKey);

    // Шифруем AES ключ с помощью публичного ключа клиента
    QByteArray encryptedAes = RSA::encryptBytes(aesKey, clientPublicKey);
    qDebug() << "Encrypted AES key:" << encryptedAes.toHex();

    responseData["aes_key"] = QString(encryptedAes.toBase64());
    qDebug() << "Base64 encoded encrypted key:" << responseData["aes_key"].toString();

    // Отправляем зашифрованный AES ключ клиенту
    response["action"] = "server_aes_key";
    response["data"] = responseData;

    return response;
}

