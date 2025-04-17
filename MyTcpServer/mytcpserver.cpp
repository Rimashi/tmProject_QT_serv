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


MyTcpServer::MyTcpServer(QObject *parent)
    : QObject(parent)
{
    mTcpServer = new QTcpServer(this);

    // Используем синглтон для базы данных
    Database *db = Database::instance();
    if (!db->initDatabase()) {
        qDebug() << "Database initialization failed!";
        return;
    }

    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 55555)) {
        qDebug() << "Server failed to start!";
    } else {
        qDebug() << "Server started on port 55555";
    }
}

MyTcpServer::~MyTcpServer()
{
    mTcpServer->close();
}

void MyTcpServer::slotNewConnection()
{
    QTcpSocket *clientSocket = mTcpServer->nextPendingConnection();
    qDebug() << "New client connected from" << clientSocket->peerAddress().toString();

    connect(clientSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
    connect(clientSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotReadyRead);

    clients.append(clientSocket);
}

void MyTcpServer::slotClientDisconnected()
{
    qDebug() << "A client disconnected.";
}

void MyTcpServer::slotReadyRead()
{
    // Получаем сокет, с которого поступили данные
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());

    // Если получен не сокет, выходим
    if (!clientSocket)
        return;

    // Читаем все данные, которые поступили в сокет, и добавляем их в буфер
    buffers[clientSocket].append(clientSocket->readAll());

    // Пока в буфере есть хотя бы одно полное сообщение, обрабатываем его
    int endIndex;
    while ((endIndex = buffers[clientSocket].indexOf("\n\n")) != -1) {
        // Извлекаем полное сообщение
        QByteArray message = buffers[clientSocket].left(endIndex);

        // Удаляем обработанное сообщение из буфера
        buffers[clientSocket].remove(0, endIndex + 2); // Убираем \n\n

        // Логируем полученное полное сообщение
        qDebug() << "Получено полное сообщение:" << message;

        // Создаем объект для парсинга JSON
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(message, &parseError);

        // Если произошла ошибка при парсинге JSON, выводим ошибку и пропускаем это сообщение
        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "Ошибка парсинга JSON:" << parseError.errorString();
            continue;
        }

        // Если парсинг прошел успешно, получаем JSON-объект
        QJsonObject obj = doc.object();

        // Извлекаем поле "action", которое определяет, какое действие запрашивает клиент
        QString action = obj["action"].toString();

        if (action == "login") {
            QString login = obj["login"].toString();
            QString password = obj["password"].toString();

            qDebug() << "Пытается войти:" << login << password;

            QJsonObject response;
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
                    qDebug()<<sessionId;
                } else {
                    response["status"] = "error";
                    response["message"] = "Ошибка создания сессии";
                }
            } else {
                response["status"] = "error";
                response["message"] = "Неверный логин или пароль";
            }

            QByteArray responseData = QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n\n";
            clientSocket->write(responseData);
        }
        else if (action == "register") {
            qDebug() << "Регистрация:" << obj;

            QString surname = obj["surname"].toString();
            QString name = obj["name"].toString();
            QString login = obj["login"].toString();
            QString password = obj["password"].toString();

            QJsonObject response;
            response["action"] = "register";

            if (Database::instance()->addNewUser(login, password, surname, name)) {
                response["status"] = "ok";
            } else {
                response["status"] = "error";
                response["message"] = "Регистрация не удалась (возможно, логин уже занят)";
            }

            QByteArray responseData = QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n\n";
            clientSocket->write(responseData);

        } else if (action == "logout"){
            QString token = obj["session_token"].toString();
            qDebug()<<Database::instance()->removeSessionFromDB(token);
        }
        else {
            qDebug() << "Неизвестное действие:" << action;
        }
    }
}
