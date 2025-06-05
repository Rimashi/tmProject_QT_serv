// client_api.h
#ifndef CLIENT_API_H
#define CLIENT_API_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include "rsa.h"

class Func2Client;

class ClientAPI : public QObject {
    Q_OBJECT
public:
    explicit ClientAPI(Func2Client* funcClient, QObject *parent = nullptr);
    void connectToServer(const QString &host, quint16 port);
    void sendLogin(const QString &login, const QString &password);
    void sendRegister(const QString &surname, const QString &name,
                      const QString &login, const QString &password);
    bool isConnected() const;
    void disconnectFromServer();
    void sendPublicKey();

    void getStatisticsStudent(const QString& login);
    void getTest1(const QString& login);
    void getTest2(const QString& login);
    void sendTest1(const QString& login, const QString& task, const QString& answer);
    void sendTest2(const QString& login, const QString& task, const QByteArray& picture);

    void getUsersAdmin();
    void getStatisticsAdmin();
    void getUserInfo(const QString& login);

signals:
    void loginSuccess(const QJsonObject &userData);
    void loginFailed(const QString &error);
    void registerSuccess();
    void registerFailed(const QString &error);
    void publicKeySent();  // Сигнал об отправке ключа
    void aesKeyReceived(const QByteArray &aesKey);  // Сигнал о получении AES

    void statisticsReceived(const QJsonObject& data);
    void task1Received(const QString& task);
    void task2Received(const QString& task);

    void usersListReceived(const QJsonArray& users);
    void adminStatisticsReceived(const QJsonArray& stats);
    void userStatisticsReceived(const QJsonObject& userData);

private slots:
    void onReadyRead();

private:
    QTcpSocket *socket;
    Func2Client* func2Client;
    // RSA::KeyPair clientKeys;  // Хранение ключей клиента
    void sendRequest(const QJsonObject &request);
};

#endif // CLIENT_API_H
