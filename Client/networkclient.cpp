#include "networkclient.h"
#include <QJsonDocument>
#include <QHostAddress>
#include <QDebug>
#include <QSettings>

NetworkClient::NetworkClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &NetworkClient::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &NetworkClient::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkClient::onReadyRead);

    connect(socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError) {
        emit errorOccurred(socket->errorString());
    });
}

NetworkClient::~NetworkClient()
{
    disconnectFromServer();
    delete socket;
}

void NetworkClient::connectToServer(const QString &host, quint16 port)
{
    socket->connectToHost(host, port);
}

void NetworkClient::disconnectFromServer()
{
    if (socket->state() == QTcpSocket::ConnectedState) {
        if (hasSession()) {
            logout();
        }
        socket->disconnectFromHost();
    }
}

bool NetworkClient::hasSession() const {
    return !sessionToken.isEmpty();
}


void NetworkClient::logout()
{
    if (!sessionToken.isEmpty()) {
        QJsonObject request;
        request["action"] = "logout";
        request["session_token"] = sessionToken;
        sendRequest(request);
        sessionToken.clear();

        QSettings settings;
        settings.remove("session_token");
    }
}


bool NetworkClient::isConnected() const
{
    return socket && socket->state() == QTcpSocket::ConnectedState;
}

void NetworkClient::sendLogin(const QString &login, const QString &password)
{
    QJsonObject request;
    request["action"] = "login";
    request["login"] = login;
    request["password"] = password;

    sendRequest(request);
}

NetworkClient* NetworkClient::getinstance()
{
    if (!instance)
        instance = new NetworkClient;
    return instance;
}

void NetworkClient::sendRegister(const QString &surname, const QString &name,
                                 const QString &login, const QString &password)
{
    QJsonObject request;
    request["action"] = "register";
    request["surname"] = surname;
    request["name"] = name;
    request["login"] = login;
    request["password"] = password;

    sendRequest(request);
}

void NetworkClient::sendRequest(const QJsonObject &request)
{
    if (!isConnected()) {
        emit errorOccurred("Не подключено к серверу");
        return;
    }

    QString action = request.value("action").toString();
    if (action != "login" && action != "register" && !sessionToken.isEmpty()) {
        request["session_token"] = sessionToken;
    }


    QByteArray data = QJsonDocument(request).toJson(QJsonDocument::Compact) + "\n\n"; // <-- компакт + \n\n
    socket->write(data);
}


void NetworkClient::onConnected()
{
    emit connected();
}

void NetworkClient::onDisconnected()
{
    emit disconnected();
}

void NetworkClient::onReadyRead()
{
    buffer.append(socket->readAll());

    int endIndex;
    while ((endIndex = buffer.indexOf("\n\n")) != -1) {
        QByteArray message = buffer.left(endIndex);
        buffer.remove(0, endIndex + 2); // удаляем \n\n тоже

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(message, &error);

        if (error.error != QJsonParseError::NoError || !doc.isObject()) {
            emit errorOccurred("Неверный формат ответа сервера");
            continue;
        }

        QJsonObject response = doc.object();
        QString action = response["action"].toString();

        if (response.contains("error")) {
            if (action == "login") {
                emit loginFailed(response["error"].toString());
            } else {
                emit registerFailed(response["error"].toString());
            }
        } else if (response["status"] == "ok") {
            if (action == "login") {
                sessionToken = response.value("session_token").toString(); // сохраняем токен
                QSettings settings;
                settings.setValue("session_token", sessionToken);
                qDebug()<<sessionToken;
                emit loginSuccess(response);
            } else {
                emit registerSuccess();
            }
        }
    }
}

NetworkClient* NetworkClient::instance = nullptr;
