// database.cpp
#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QJsonObject>
#include "sha384.h"

Database::Database(QObject *parent) : QObject(parent) {
    if (QSqlDatabase::contains("test")) {
        m_db = QSqlDatabase::database("test");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "test");
    }
}

Database::~Database() {
    close();
}

bool Database::open(const QString& path) {
    if (m_db.isOpen()) return true;

    m_db.setDatabaseName(path);
    qDebug() << "Opening database:" << path;
    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        qDebug() << "Database: Ошибка открытия БД:" << m_lastError;
        return false;
    }
    qDebug() << "Database opened successfully";


    QSqlQuery query(m_db);
    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "login TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL,"
        "name TEXT NOT NULL,"
        "surname TEXT NOT NULL,"
        "role TEXT DEFAULT 'student'"
        ");"
        );
    if (!success) {
        qCritical() << "Database: Ошибка создания таблицы users:" << query.lastError().text();
        return false;
    }
    return true;
}

void Database::close() {
    if (m_db.isOpen()) m_db.close();
    // QSqlDatabase::removeDatabase("test");
}

bool Database::userExists(const QString& login) {
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM users WHERE login = ?");
    query.addBindValue(login);
    if (!query.exec() || !query.next()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return query.value(0).toInt() > 0;
}

bool Database::registerUser(const QString& login, const QString& passwordHash, const QString& name, const QString& surname) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO users (login, password, name, surname, role) VALUES (?, ?, ?, ?, 'student')");
    query.addBindValue(login);
    query.addBindValue(passwordHash);
    query.addBindValue(name);
    query.addBindValue(surname);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Database: Ошибка регистрации:" << m_lastError;
        return false;
    }
    return true;
}

bool Database::validateUser(const QString& login, const QString& passwordHash, QJsonObject &userData) {
    QSqlQuery query(m_db);
    query.prepare("SELECT id, surname, name, role FROM users WHERE login = ? AND password = ?");
    query.addBindValue(login);
    query.addBindValue(passwordHash);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    if (query.next()) {
        userData["id"] = query.value("id").toInt();
        userData["lastName"] = query.value("surname").toString();
        userData["firstName"] = query.value("name").toString();
        userData["role"] = query.value("role").toString();
        return true;
    }
    return false;
}

void Database::initializeAdmin() {
    if (!userExists("admin")) {
        QString adminPassword = SHA384::hash("admin123");
        QSqlQuery query(m_db);
        query.prepare("INSERT INTO users (login, password, name, surname, role) VALUES (?, ?, ?, ?, ?)");
        query.addBindValue("admin");
        query.addBindValue(adminPassword);
        query.addBindValue("Admin");
        query.addBindValue("Admin");
        query.addBindValue("admin");
        if (query.exec()) {
            qDebug() << "Database: Админ создан (login: admin, пароль: admin123)";
        } else {
            qCritical() << "Database: Ошибка создания админа:" << query.lastError().text();
        }
    }
}

QString Database::lastError() const {
    return m_lastError;
}
