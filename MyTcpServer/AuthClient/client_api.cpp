// client_api.cpp
#include "client_api.h"
#include "func2client.h"
#include <QJsonDocument>
#include <QJsonArray>

ClientAPI::ClientAPI(Func2Client* f2c, QObject *parent)
    : QObject(parent), func2Client(f2c)
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &ClientAPI::onReadyRead);

    // Подключаем сигнал успешного соединения
    connect(socket, &QTcpSocket::connected, this, [this]() {
        qDebug() << "Connected to server, sending public key...";
        sendPublicKey(); // Отправляем ключ сразу после подключения
    });
}

bool ClientAPI::isConnected() const {
    return socket->state() == QAbstractSocket::ConnectedState;
}

void ClientAPI::connectToServer(const QString& host, quint16 port) {
    socket->connectToHost(host, port);
}

void ClientAPI::disconnectFromServer() {
    if (socket->state() != QAbstractSocket::UnconnectedState) {
        socket->disconnectFromHost();
    }
}

void ClientAPI::sendLogin(const QString& login, const QString& password) {
    QJsonObject request = func2Client->prepareLogin(login, password);
    sendRequest(request);
}

void ClientAPI::sendRegister(const QString& surname, const QString& name,
                             const QString& login, const QString& password) {
    QJsonObject request = func2Client->prepareRegister(surname, name, login, password);
    sendRequest(request);
}

void ClientAPI::sendRequest(const QJsonObject& json) {
    QJsonDocument doc(json);
    socket->write(doc.toJson(QJsonDocument::Compact) + '\n');
    socket->flush();
}

void ClientAPI::sendPublicKey() {
    QJsonObject data;
    data["public_key"] = func2Client->getRsaKeys().publicKey;

    QJsonObject request;
    request["action"] = "client_public_key";
    request["data"] = data;

    sendRequest(request);
    emit publicKeySent();
}

void ClientAPI::getStatisticsStudent(const QString& login) {
    QJsonObject data;
    data["login"] = login; // Используем "login" вместо "id/login"

    QJsonObject request;
    request["action"] = "get_statistics_student";
    request["data"] = data;

    sendRequest(func2Client->encryptRequestData(request)); // Отправляем шифрованный запрос
}

void ClientAPI::getTest1(const QString& login) {
    QJsonObject data;
    data["login"] = login;

    QJsonObject request;
    request["action"] = "get_test1";
    request["data"] = data;

    sendRequest(func2Client->encryptRequestData(request));
}

void ClientAPI::getTest2(const QString& login) {
    QJsonObject data;
    data["login"] = login;

    QJsonObject request;
    request["action"] = "get_test2";
    request["data"] = data;

    sendRequest(func2Client->encryptRequestData(request));
}

void ClientAPI::sendTest1(const QString& login, const QString& task, const QString& answer) {
    QJsonObject data;
    data["login"] = login;
    data["task"] = task;
    data["answer"] = answer;

    QJsonObject request;
    request["action"] = "send_test1";
    request["data"] = data;

    sendRequest(func2Client->encryptRequestData(request));
}

void ClientAPI::sendTest2(const QString& login, const QString& task, const QByteArray& picture) {
    QJsonObject data;
    data["login"] = login;
    data["task"] = task;
    data["picture"] = QString(picture.toBase64());

    QJsonObject request;
    request["action"] = "send_test2";
    request["data"] = data;

    sendRequest(func2Client->encryptRequestData(request));
}

void ClientAPI::getUsersAdmin() {
    QJsonObject data;
    data["dummy"] = "dummy"; // передаем что-нибудь

    QJsonObject request;
    request["action"] = "get_users_admin";
    request["data"] = data;

    sendRequest(func2Client->encryptRequestData(request));
}

void ClientAPI::getStatisticsAdmin() {
    QJsonObject data;
    data["dummy"] = "dummy";

    QJsonObject request;
    request["action"] = "get_statistics_admin";
    request["data"] = data;

    sendRequest(func2Client->encryptRequestData(request));
}

void ClientAPI::getUserInfo(const QString& login) {
    QJsonObject data;
    data["login"] = login;

    QJsonObject request;
    request["action"] = "get_user_info";
    request["data"] = data;

    sendRequest(func2Client->encryptRequestData(request));
}

void ClientAPI::onReadyRead() {
    while (socket->canReadLine()) {
        QByteArray data = socket->readLine().trimmed();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        qDebug() << "Raw response data:" << data;

        if (doc.isNull() || !doc.isObject()) {
            qWarning() << "Invalid JSON response";
            continue;
        }

        QJsonObject obj = doc.object();
        QString action = obj["action"].toString();
        QJsonObject dataObj = obj["data"].toObject();

        if (action == "login_result") {
            if (dataObj["success"].toBool()) {
                QString role = dataObj["role"].toString();
                QString login = dataObj["login"].toString();
                QString name = dataObj["name"].toString();
                QString surname = dataObj["surname"].toString();
                qDebug() << role << login << name << surname;
                emit loginSuccess(dataObj);
            } else {
                emit loginFailed(dataObj["error"].toString());
            }
        }else if (action == "register_result") {
            if (dataObj["success"].toBool()) {
                emit registerSuccess();
            } else {
                emit registerFailed(obj["error"].toString());
            }
        }else if (action == "server_aes_key") {
            // QJsonObject dataObj = obj["data"].toObject();
            if (func2Client->handleAesKeyResponse(dataObj)) {
                qDebug() << "AES key successfully processed";
                // Можно отправить сигнал, если нужно
            } else {
                qWarning() << "Failed to process AES key";
            }
        }else if (action == "send_statistics_student") {
            emit statisticsReceived(dataObj); // Передаем данные в StudentMainScreen
        }else if (action == "send_task1") {
            // Проверяем наличие поля "task"
            if (dataObj.contains("task")) {
                emit task1Received(dataObj["task"].toString());
            } else {
                qWarning() << "Invalid task1 response format";
            }
        }else if (action == "check_test1_result") {
            if (dataObj["success"].toBool()) {
                qDebug() << "Test1 answer submitted successfully";//не особо имеет смысл
            } else {
                qWarning() << "Test1 submission failed:" << dataObj["error"].toString();
            }
        }else if (action == "send_task2") {
            emit task2Received(dataObj["task"].toString());
        }else if (action == "check_test2_result"){
            if (dataObj["success"].toBool()) {
                qDebug() << "Test2 submitted successfully. Score:" << dataObj["score"].toInt();
            } else {
                qWarning() << "Test2 submission failed:" << dataObj["error"].toString();
            }
        }else if (action == "send_users_admin"){
            if (dataObj.contains("users") && dataObj["users"].isArray()) {
                emit usersListReceived(dataObj["users"].toArray());
            }
        }else if (action == "send_statistics_admin") {
            if (dataObj.contains("statistics") && dataObj["statistics"].isArray()) {
                emit adminStatisticsReceived(dataObj["statistics"].toArray());
            }
        }else if (action == "send_user_info") {
            emit userStatisticsReceived(dataObj);
        }
    }
}
