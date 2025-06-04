// server_api.cpp
#include "server_api.h"
#include "func2serv.h"
#include "rsa.h"
#include "aes.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

Server_api::Server_api(QObject *parent) : QTcpServer(parent) {
    func2Serv = new Func2Serv(this, this); // Передаем себя в конструктор
    setupDatabase();

    if (listen(QHostAddress::Any, 3000)) {
        qDebug() << "Server started on port 3000";
    } else {
        qCritical() << "Failed to start server:" << errorString();
    }
}

void Server_api::addAesKey(QTcpSocket* socket, const QByteArray& key) {
    clientAesKeys[socket] = key;
}

void Server_api::setupDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("server.db");

    if (!db.open()) {
        qCritical() << "DB error:" << db.lastError().text();
        return;
    }

    // Создаем таблицу если не существует
    QSqlQuery q;
    if (!q.exec("CREATE TABLE IF NOT EXISTS users ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "surname TEXT NOT NULL,"
                "name TEXT NOT NULL,"
                "login TEXT UNIQUE NOT NULL,"
                "password TEXT NOT NULL)")) {
        qCritical() << "DB table creation error:" << q.lastError().text();
    }
}

void Server_api::incomingConnection(qintptr socketDescriptor) {
    qDebug() << "New incoming connection. Descriptor:" << socketDescriptor;

    QTcpSocket* socket = new QTcpSocket(this);
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        qWarning() << "Failed to set socket descriptor!";
        socket->deleteLater();
        return;
    }

    qDebug() << "Client connected from" << socket->peerAddress().toString();

    connect(socket, &QTcpSocket::readyRead, [this, socket]() {
        QByteArray data = socket->readAll();
        qDebug() << "Received data:" << data;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "JSON parse error:" << parseError.errorString();
            socket->write("{\"status\": \"error\", \"message\": \"Invalid JSON\"}");
            socket->flush();
            return;
        }

        QJsonObject request = doc.object();
        QString action = request["action"].toString();
        qDebug() << "Action:" << action;

        QJsonObject response;

        if (action == "client_public_key") {
            qDebug()<<"trying to get keys";
            QJsonObject JsonData = request["data"].toObject();
            if (JsonData["public_key"].toString().isEmpty()) {
                qCritical() << "Empty public key received from client";
                response["action"] = "error";
                response["message"] = "Empty public key";
            } else {
                response = func2Serv->handlePublicKey(JsonData, socket);
            }
        }else{
            // Проверяем наличие AES-ключа для сокета
            if (!clientAesKeys.contains(socket)) {
                response["action"] = "error";
                response["message"] = "AES key not established";
                qWarning() << "No AES key for socket";
            }else{
                // Получаем зашифрованные данные (строку в base64)
                QString encryptedBase64 = request["data"].toString();
                QByteArray encryptedData = QByteArray::fromBase64(encryptedBase64.toUtf8());

                // Расшифровываем данные
                QByteArray decryptedData = AES::decrypt(encryptedData, clientAesKeys[socket]);
                QJsonDocument dataDoc = QJsonDocument::fromJson(decryptedData);

                if (dataDoc.isNull()) {
                    response["action"] = "error";
                    response["message"] = "Invalid decrypted data format";
                    qWarning() << "Failed to parse decrypted JSON";
                } else {
                    QJsonObject dataForHandler = dataDoc.object();
                    qDebug() << "Decrypted data:" << dataForHandler;

                    if (action == "register") {
                        qDebug() << "trying to register";
                        response = func2Serv->handleRegistration(dataForHandler);
                    } else if (action == "login") {
                        qDebug() << "trying to login";
                        response = func2Serv->handleLogin(dataForHandler);
                    } else {
                        response["action"] = "error";
                        response["message"] = "Unknown action";
                        qWarning() << "Unknown action received:" << action;
                    }
                }
            }
        }
        QByteArray responseData = QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n";
        qDebug() << "Sending response:" << responseData;
        socket->write(responseData);
        socket->flush();
    });

    connect(socket, &QTcpSocket::disconnected, [this, socket]() {
        qDebug() << "Client disconnected";
        clientAesKeys.remove(socket);
        socket->deleteLater();
    });
}
