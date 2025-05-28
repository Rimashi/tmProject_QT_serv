#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonArray>

class NetworkClient : public QObject
{
    Q_OBJECT
public:
    static NetworkClient* getinstance();
    ~NetworkClient();

    void connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();
    void sendLogin(const QString &login, const QString &password);
    void sendRegister(const QString &surname, const QString &name,
                      const QString &login, const QString &password);
    bool isConnected() const;
    void logout();
    bool hasSession() const;

signals:
    void connected();
    void disconnected();
    void loginSuccess(const QJsonObject &userData);
    void loginFailed(const QString &error);
    void registerSuccess();
    void registerFailed(const QString &error);
    void errorOccurred(const QString &error);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();

private:
    static NetworkClient* instance;
    explicit NetworkClient(QObject *parent = nullptr);

    QTcpSocket *socket;
    QByteArray buffer;
    QString sessionToken;
    void sendRequest(const QJsonObject &request);

};

#endif // NETWORKCLIENT_H
