/*
#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>

class NetworkClient : public QObject
{
    Q_OBJECT

private:
    explicit NetworkClient(QObject *parent = nullptr);
    NetworkClient(const NetworkClient&) = delete;
    NetworkClient& operator=(const NetworkClient&) = delete;
    ~NetworkClient();

public:
    static NetworkClient* instance();

    // Основные методы
    void connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();
    bool isConnected() const;

    // Авторизация
    void login(const QString &login, const QString &password);
    void logout();
    bool hasSession() const;

    // Регистрация
    void registerUser(const QString &surname, const QString &name,
                      const QString &login, const QString &password);

    // Отправка произвольного запроса
    void sendRequest(const QJsonObject &request);

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);
    void loginSuccess(const QJsonObject &userData);
    void loginFailed(const QString &error);
    void registerSuccess();
    void registerFailed(const QString &error);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    static NetworkClient* m_instance;
    static QScopedPointer<NetworkClient> m_destroyer;

    QTcpSocket* m_socket;
    QByteArray m_buffer;
    QString m_sessionToken;

    void initializeSocket();
    void processServerResponse(const QJsonObject &response);
};

#endif // NETWORKCLIENT_H
*/
