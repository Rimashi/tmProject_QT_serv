/*
#include "networkclient.h"
#include <QJsonDocument>
#include <QDebug>

NetworkClient* NetworkClient::m_instance = nullptr;
QScopedPointer<NetworkClient> NetworkClient::m_destroyer;

NetworkClient::NetworkClient(QObject *parent) : QObject(parent)
{
    initializeSocket();
}

NetworkClient::~NetworkClient()
{
    disconnectFromServer();
}

NetworkClient* NetworkClient::instance()
{
    if (!m_instance) {
        m_instance = new NetworkClient();
        m_destroyer.reset(m_instance);
    }
    return m_instance;
}

void NetworkClient::initializeSocket()
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::connected, this, &NetworkClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkClient::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkClient::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &NetworkClient::onErrorOccurred);
}

void NetworkClient::connectToServer(const QString &host, quint16 port)
{
    if (isConnected()) {
        return;
    }
    m_socket->connectToHost(host, port);
}

void NetworkClient::disconnectFromServer()
{
    if (isConnected()) {
        if (hasSession()) {
            logout();
        }
        m_socket->disconnectFromHost();
    }
}

bool NetworkClient::isConnected() const
{
    return m_socket && m_socket->state() == QTcpSocket::ConnectedState;
}

void NetworkClient::login(const QString &login, const QString &password)
{
    QJsonObject request{
        {"action", "login"},
        {"login", login},
        {"password", password}
    };
    sendRequest(request);
}

void NetworkClient::logout()
{
    if (hasSession()) {
        QJsonObject request{
            {"action", "logout"},
            {"session_token", m_sessionToken}
        };
        sendRequest(request);
        m_sessionToken.clear();
        QSettings().remove("session_token");
    }
}

void NetworkClient::registerUser(const QString &surname, const QString &name,
                                 const QString &login, const QString &password)
{
    QJsonObject request{
        {"action", "register"},
        {"surname", surname},
        {"name", name},
        {"login", login},
        {"password", password}
    };
    sendRequest(request);
}

void NetworkClient::sendRequest(const QJsonObject &request)
{
    if (!isConnected()) {
        emit connectionError("Not connected to server");
        return;
    }

    QJsonObject modifiedRequest = request;
    if (request["action"].toString() != "login" &&
        request["action"].toString() != "register" &&
        hasSession()) {
        modifiedRequest["session_token"] = m_sessionToken;
    }

    QByteArray data = QJsonDocument(modifiedRequest).toJson() + "\n";
    m_socket->write(data);
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
    m_buffer.append(m_socket->readAll());

    while (true) {
        int endPos = m_buffer.indexOf('\n');
        if (endPos == -1) break;

        QByteArray message = m_buffer.left(endPos);
        m_buffer.remove(0, endPos + 1);

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(message, &error);

        if (error.error != QJsonParseError::NoError || !doc.isObject()) {
            emit connectionError("Invalid server response");
            continue;
        }

        processServerResponse(doc.object());
    }
}

void NetworkClient::onErrorOccurred(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    emit connectionError(m_socket->errorString());
}

void NetworkClient::processServerResponse(const QJsonObject &response)
{
    QString action = response["action"].toString();

    if (response.contains("error")) {
        if (action == "login") {
            emit loginFailed(response["error"].toString());
        } else if (action == "register") {
            emit registerFailed(response["error"].toString());
        }
        return;
    }

    if (response["status"] == "ok") {
        if (action == "login") {
            m_sessionToken = response["session_token"].toString();
            QSettings().setValue("session_token", m_sessionToken);
            emit loginSuccess(response);
        } else if (action == "register") {
            emit registerSuccess();
        }
    }
}

bool NetworkClient::hasSession() const
{
    return !m_sessionToken.isEmpty();
}
*/
