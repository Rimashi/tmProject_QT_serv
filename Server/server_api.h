// server_api.h
#ifndef SERVER_API_H
#define SERVER_API_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>
#include <QSqlDatabase>
#include <QMap>

class Func2Serv;

class Server_api : public QTcpServer {
    Q_OBJECT
public:
    explicit Server_api(QObject *parent = nullptr);
    void addAesKey(QTcpSocket* socket, const QByteArray& key);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    void setupDatabase();
    QSqlDatabase db;
    Func2Serv* func2Serv;
    QMap<QTcpSocket*, QByteArray> clientAesKeys; // Хранилище ключей
};

#endif // SERVER_API_H
