#include "mytcpserver.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostAddress>
#include <QDebug>
#include "database.h"
#include <QJsonArray>
#include "utils.h"
#include <QSqlQuery>
#include <QSqlError>
#include "func2serv.h"

MyTcpServer::MyTcpServer(QObject *parent)
    : QObject(parent)
{
    qDebug() << "Инициализация сервера...";

    QTcpSocket testSocket;
    testSocket.connectToHost(QHostAddress::LocalHost, 55555);
    if (testSocket.waitForConnected(1000)) {
        qCritical() << "Порт 55555 уже занят!";
        testSocket.disconnectFromHost();
        return;
    }

    try {
        qDebug() << "Генерация ключей RSA...";
        serverKeys = RSA::generateKeys(512);
        qDebug() << "Ключи успешно сгенерированы";
        qDebug() << "Публичный ключ:" << serverKeys.publicKey.left(50) << "..."; // Первые 50 символов
        qDebug() << "Приватный ключ:" << serverKeys.privateKey.left(50) << "...";
    } catch (const std::exception& e) {
        qCritical() << "Ошибка генерации ключей RSA:" << e.what();
        return;
    }

    qDebug() << "Создание QTcpServer...";
    mTcpServer = new QTcpServer(this);

    qDebug() << "Инициализация базы данных...";
    Database::instance();
    qDebug() << "База данных инициализирована";

    qDebug() << "Запуск сервера на порту 55555...";
    if (!mTcpServer->listen(QHostAddress::Any, 55555)) {
        qCritical() << "Ошибка запуска сервера:" << mTcpServer->errorString();
    } else {
        qDebug() << "Сервер запущен на порту 55555";
    }

    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);
}

MyTcpServer::~MyTcpServer()
{
    for (QTcpSocket* client : clients) {
        client->disconnectFromHost();
    }
    mTcpServer->close();
}

void MyTcpServer::slotNewConnection()
{
    qDebug() << "Новое подключение клиента";
    QTcpSocket* clientSocket = mTcpServer->nextPendingConnection();
    clients.append(clientSocket);

    // Отправляем клиенту публичный ключ сервера
    qDebug() << "Формирование сообщения с ключом...";
    QJsonObject keyMsg;
    keyMsg["action"] = "key_exchange";
    keyMsg["public_key"] = serverKeys.publicKey;
    QString jsonStr = QJsonDocument(keyMsg).toJson(QJsonDocument::Compact);
    qDebug() << "JSON сформирован:" << jsonStr;

    qDebug() << "Отправка ключа клиенту...";
    clientSocket->write((jsonStr + "\n\n").toUtf8());
    qDebug() << "Ключ отправлен";

    connect(clientSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
}

void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        clients.removeAll(clientSocket);
        buffers.remove(clientSocket);
        clientSocket->deleteLater();
        qDebug() << "Клиент отключился";
    }
}

void MyTcpServer::slotReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    buffers[clientSocket].append(clientSocket->readAll());

    int endIndex;
    while ((endIndex = buffers[clientSocket].indexOf("\n\n")) != -1) {
        QByteArray encryptedMessage = buffers[clientSocket].left(endIndex);
        buffers[clientSocket].remove(0, endIndex + 2);

        qDebug() << "Получено зашифрованное сообщение, длина:" << encryptedMessage.size();
        // Дешифровка сообщения
        QString decrypted = RSA::decrypt(QString::fromUtf8(encryptedMessage), serverKeys.privateKey);
        qDebug() << "Сообщение расшифровано:" << decrypted.left(100) << "...";

        QJsonDocument doc = QJsonDocument::fromJson(decrypted.toUtf8());
        if (doc.isNull() || !doc.isObject()) {
            qDebug() << "Ошибка парсинга JSON";
            continue;
        }

        QJsonObject obj = doc.object();
        QString action = obj["action"].toString();
        QJsonObject response;

        if (action == "login") {
            QString login = obj["login"].toString();
            QString password = obj["password"].toString();

            qDebug() << "Пытается войти:" << login;

            response["action"] = "login";
            QJsonObject userData;
            if (Database::instance()->authenticateUser(login, password, userData)) {
                QString sessionId = Database::instance()->createSessionForUser(login);
                if (!sessionId.isEmpty()) {
                    response["status"] = "ok";
                    response["surname"] = userData["surname"];
                    response["name"] = userData["name"];
                    response["role"] = userData["role"];
                    response["session_token"] = sessionId;
                    qDebug() << "Сессия создана:" << sessionId;
                } else {
                    response["status"] = "error";
                    response["message"] = "Ошибка создания сессии";
                }
            } else {
                response["status"] = "error";
                response["message"] = "Неверный логин или пароль";
            }
        }
        else if (action == "register") {
            qDebug() << "Регистрация:" << obj;

            QString surname = obj["surname"].toString();
            QString name = obj["name"].toString();
            QString login = obj["login"].toString();
            QString password = obj["password"].toString();

            response["action"] = "register";
            if (Database::instance()->addNewUser(login, password, surname, name)) {
                response["status"] = "ok";
            } else {
                response["status"] = "error";
                response["message"] = "Регистрация не удалась (возможно, логин уже занят)";
            }
        }
        else if (action == "logout") {
            QString token = obj["session_token"].toString();
            response["action"] = "logout";
            QString result = Database::instance()->removeSessionFromDB(token);
            response["status"] = result == "success" ? "ok" : "error";
            if (result == "error") {
                response["message"] = "Ошибка удаления сессии";
            }
            qDebug() << "Выход из сессии:" << token << result;
        }
        else if (action == "get_tests" || action == "submit_test" || action == "get_statistics" ||
                 action == "get_users" || action == "get_user_stats") {
            // Делегируем обработку тестов и статистики Func2Serv
            Func2Serv func2Serv;
            if (action == "get_tests") {
                response = func2Serv.handleGetTests(obj["session_token"].toString());
            } else if (action == "submit_test") {
                response = func2Serv.handleSubmitTest(obj["session_token"].toString(),
                                                      obj["test_number"].toInt(),
                                                      obj["answer"].toString());
            } else if (action == "get_statistics") {
                response = func2Serv.handleGetStatistics(obj["session_token"].toString());
            } else if (action == "get_users") {
                response = func2Serv.handleGetUsers(obj["session_token"].toString());
            } else if (action == "get_user_stats") {
                response = func2Serv.handleGetUserStats(obj["session_token"].toString(),
                                                        obj["user_id"].toInt());
            }
        }
        else {
            qDebug() << "Неизвестное действие:" << action;
            response["action"] = action;
            response["status"] = "error";
            response["message"] = "Неизвестное действие";
        }

        qDebug() << "Формирование ответа...";
        QString encryptedResponse = RSA::encrypt(QJsonDocument(response).toJson(QJsonDocument::Compact), obj["client_key"].toString());
        qDebug() << "Отправка ответа клиенту...";
        clientSocket->write((encryptedResponse + "\n\n").toUtf8());
        qDebug() << "Ответ отправлен";
    }
}
