#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include "utils.h"

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();

private slots:
    void slotNewConnection();
    void slotClientDisconnected();

private:
    RSA::KeyPair serverKeys;
    QTcpServer *mTcpServer;
    QList<QTcpSocket*> clients;

    QMap<QTcpSocket*, QByteArray> buffers;

private slots:
    void slotReadyRead();

};

#endif // MYTCPSERVER_H
