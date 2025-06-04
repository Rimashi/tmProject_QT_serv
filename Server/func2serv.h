#ifndef FUNC2SERV_H
#define FUNC2SERV_H

#include <QObject>
#include <QJsonObject>
#include "database.h"
#include <QTcpSocket>
#include <QByteArray>

class Server_api; // Forward declaration

class Func2Serv : public QObject {
    Q_OBJECT
public:
    explicit Func2Serv(Server_api* serverApi, QObject *parent = nullptr);

    QJsonObject handleRegistration(const QJsonObject &data);
    QJsonObject handleLogin(const QJsonObject &data);
    QJsonObject handlePublicKey(const QJsonObject &data, QTcpSocket* socket);

private:
    Database& db = Database::instance();
    Server_api* serverApi; // Ссылка на сервер типа
};

#endif // FUNC2SERV_H
