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
    QJsonObject get_user_statistic(const QJsonObject &data);
    QJsonObject get_users_admin();
    QJsonObject get_statistics_admin();
    QJsonObject get_user_info(const QJsonObject &data);

    QJsonObject checkTest1(const QJsonObject &data);
    QString generateRandomEquation();
    QJsonObject generateTest1Task(const QJsonObject &data);
    double solveChordFromString(const QString& eqn, double eps);

    QString generateRandomBitString(int length);
    QJsonObject generateTest2Task(const QJsonObject &data);
    QJsonObject checkTest2(const QJsonObject &data);
    QString extractBitsFromImage(const QImage &image, int bitCount);

private:
    Database& db = Database::instance();
    Server_api* serverApi; // Ссылка на сервер типа
};

#endif // FUNC2SERV_H
