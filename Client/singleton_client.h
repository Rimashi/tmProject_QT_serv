#ifndef SINGLETON_CLIENT_H
#define SINGLETON_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonArray>

class Singleton_client;

// Destroyer
class Singleton_clientDestroyer
{
private:
    Singleton_client* pInstance = nullptr;
public:
    ~Singleton_clientDestroyer();
    void initialize(Singleton_client *p);
};

// Singleton
class Singleton_client : public QObject
{
    Q_OBJECT

private:
    static Singleton_client* pInstance;
    static Singleton_clientDestroyer destroyer;
    static QTcpSocket* tcpInstance;

    explicit Singleton_client(QObject *parent = nullptr);
    Singleton_client(const Singleton_client&) = delete;
    Singleton_client& operator=(const Singleton_client&) = delete;
    ~Singleton_client();

    void initTCP();

    friend class Singleton_clientDestroyer;

public:
    static Singleton_client* getInstance();
    QString send_request(const QJsonArray& request);
    void connectToServer(const QString& host, quint16 port);

signals:
    void dataReceived(const QByteArray& data);

private slots:
    void slot_on_read();
    void slot_on_disconnected();
};

#endif // SINGLETON_CLIENT_H
