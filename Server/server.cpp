#include "server.h"
#include "database.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>

Server::Server(QObject *parent) : QTcpServer(parent) {
    if (!listen(QHostAddress::Any, 3000)) {
        qCritical() << "Can't listen on port 3000";
    } else {
        qDebug() << "Server started on port 3000";
    }
    setupDatabase();
}

void Server::setupDatabase() { // work with database, only retarget functions, calling....
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("server.db");
    if (!db.open()) {
        qCritical() << "DB open error:" << db.lastError().text();
        return;
    }

    QSqlQuery q;
    if (!q.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, login TEXT UNIQUE, password TEXT)")) {
        qCritical() << "DB table creation failed:" << q.lastError().text();
    } else {
        qDebug() << "DB is ready (users table created if not exists)";
    }
}

QString Server::hashPassword(const QString &password) { // call functions from aes,rsa
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha384).toHex());
}

void Server::incomingConnection(qintptr socketDescriptor) {
    qDebug() << "New incoming connection. Descriptor:" << socketDescriptor;

    auto *socket = new QTcpSocket(this);
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        qWarning() << "Failed to set socket descriptor!";
        socket->deleteLater();
        return;
    }

    qDebug() << "Client connected from" << socket->peerAddress().toString();

    connect(socket, &QTcpSocket::readyRead, [this, socket]() {
        QByteArray data = socket->readAll();
        qDebug() << "Received data:" << data;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "JSON parse error:" << parseError.errorString();
            socket->write("{\"status\": \"error\", \"message\": \"Invalid JSON\"}");
            return;
        }

        QJsonObject obj = doc.object();
        QString action = obj["action"].toString();
        qDebug() << "Action:" << action;

        QJsonObject reply;

        if (action == "register") {
            QString login = obj["login"].toString();
            QString password = hashPassword(obj["password"].toString());

            qDebug() << "Registering user:" << login;

            QSqlQuery q;
            q.prepare("INSERT INTO users (login, password) VALUES (?, ?)");
            q.addBindValue(login);
            q.addBindValue(password);

            if (q.exec()) {
                reply["status"] = "ok";
                qDebug() << "User registered:" << login;
            } else {
                reply["status"] = "error";
                reply["message"] = "User exists or DB error";
                qWarning() << "Register failed:" << q.lastError().text();
            }

        } else if (action == "login") {
            QString login = obj["login"].toString();
            QString password = hashPassword(obj["password"].toString());

            qDebug() << "Attempt login for:" << login;

            QSqlQuery q;
            q.prepare("SELECT id FROM users WHERE login = ? AND password = ?");
            q.addBindValue(login);
            q.addBindValue(password);

            if (!q.exec()) {
                qWarning() << "Login query failed:" << q.lastError().text();
                reply["status"] = "error";
                reply["message"] = "Database error";
            } else if (q.next()) {
                reply["status"] = "ok";
                qDebug() << "Login success for:" << login;
            } else {
                reply["status"] = "error";
                reply["message"] = "Invalid credentials";
                qDebug() << "Login failed: invalid credentials";
            }

        } else {
            reply["status"] = "error";
            reply["message"] = "Unknown action";
            qWarning() << "Unknown action received:" << action;
        }

        QByteArray responseData = QJsonDocument(reply).toJson();
        qDebug() << "Sending response:" << responseData;
        socket->write(responseData);
        socket->flush();
    });

    connect(socket, &QTcpSocket::disconnected, [socket]() {
        qDebug() << "Client disconnected";
        socket->deleteLater();
    });
}
