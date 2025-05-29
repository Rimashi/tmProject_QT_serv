#include "Singleton_client.h"
#include <QJsonDocument>
#include <QDebug>

// Инициализация статических членов
Singleton_client* Singleton_client::pInstance = nullptr;
Singleton_clientDestroyer Singleton_client::destroyer;
QTcpSocket* Singleton_client::tcpInstance = nullptr;

// Destroyer
Singleton_clientDestroyer::~Singleton_clientDestroyer()
{
    delete pInstance;
}

void Singleton_clientDestroyer::initialize(Singleton_client *p)
{
    pInstance = p;
}

// Singleton_client
Singleton_client::Singleton_client(QObject *parent) : QObject(parent)
{
    initTCP();
}

Singleton_client::~Singleton_client()
{
    if (tcpInstance) {
        tcpInstance->close();
        delete tcpInstance;
        tcpInstance = nullptr;
    }
}

void Singleton_client::initTCP()
{
    tcpInstance = new QTcpSocket(this);
    connect(tcpInstance, &QTcpSocket::readyRead, this, &Singleton_client::slot_on_read);
    connect(tcpInstance, &QTcpSocket::disconnected, this, &Singleton_client::slot_on_disconnected);
}

Singleton_client* Singleton_client::getInstance()
{
    if (!pInstance) {
        pInstance = new Singleton_client();
        destroyer.initialize(pInstance);
    }
    return pInstance;
}

void Singleton_client::connectToServer(const QString& host, quint16 port)
{
    if (tcpInstance)
        tcpInstance->connectToHost(host, port);
}

QString Singleton_client::send_request(const QJsonArray& request)
{
    if (!tcpInstance || !tcpInstance->isOpen()) {
        qWarning() << "Соединение с сервером не установлено!";
        return QString();
    }

    QJsonDocument doc(request);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    tcpInstance->write(data);
    tcpInstance->waitForBytesWritten();

    if (tcpInstance->waitForReadyRead(3000)) {
        QByteArray response = tcpInstance->readAll();
        return QString::fromUtf8(response);
    } else {
        qWarning() << "Не удалось получить ответ от сервера.";
        return QString();
    }
}

void Singleton_client::slot_on_read()
{
    QByteArray data = tcpInstance->readAll();
    emit dataReceived(data);
}

void Singleton_client::slot_on_disconnected()
{
    qDebug() << "Соединение с сервером разорвано.";
}
