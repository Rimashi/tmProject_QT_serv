#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QJsonObject>
#include "utils.h"

Database* Database::instance()
{
    static Database instance; // создаётся один раз
    return &instance;
}

Database::Database(QObject *parent)
    : QObject(parent)
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("exam_system.db");
    }
}

Database::~Database()
{
    if (db.isOpen()) {
        db.close();
    }
}


bool Database::initDatabase()
{
    if (!db.open()) {
        qDebug() << "Failed to open database: " << db.lastError().text();
        return false;
    }

    // Проверка наличия таблиц и создание их при необходимости
    if (!checkTableExists("user")) {
        if (!createTables()) {
            return false;
        }
    }

    // Проверка наличия админа
    if (!addAdminIfNeeded()) {
        return false;
    }

    return true;
}

bool Database::checkTableExists(const QString &tableName)
{
    QSqlQuery query;
    query.exec(QString("SELECT name FROM sqlite_master WHERE type='table' AND name='%1'").arg(tableName));
    return query.next();
}

bool Database::createTables()
{
    QSqlQuery query;
    // Создание таблицы пользователей
    query.exec("CREATE TABLE user ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT, "
               "surname TEXT, "
               "login TEXT UNIQUE, "
               "password TEXT, "
               "role TEXT)");

    // Создание таблицы сессий
    query.exec("CREATE TABLE sessions ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "userid INTEGER, "
               "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
               "FOREIGN KEY(userid) REFERENCES user(id))");

    // Создание таблицы экзаменов
    query.exec("CREATE TABLE exams ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "number INTEGER, "
               "userid INTEGER, "
               "score INTEGER, "
               "FOREIGN KEY(userid) REFERENCES user(id))");

    return true;
}

bool Database::addAdminIfNeeded()
{
    QSqlQuery query;
    query.exec("SELECT * FROM user WHERE role = 'admin'"); // да, одмен один

    if (!query.next()) {
        // Если админа нет, добавляем его
        query.prepare("INSERT INTO user (name, surname, login, password, role) "
                      "VALUES (:name, :surname, :login, :password, :role)");
        query.bindValue(":name", "Admin");
        query.bindValue(":surname", "Adminov");
        query.bindValue(":login", "admin");
        query.bindValue(":password", hashPassword("admin1")); // в идеале эти и другие чувствительные данные вынести в файлик .env
        query.bindValue(":role", "admin");

        if (!query.exec()) {
            qDebug() << "Failed to insert admin: " << query.lastError().text();
            return false;
        }
    }

    return true;
}

QString Database::createSessionForUser(const QString &login) {
    QSqlQuery query;

    query.prepare("SELECT id FROM user WHERE login = ?");
    query.addBindValue(login);
    if (!query.exec() || !query.next()) {
        qDebug() << "Не удалось найти пользователя для сессии:" << query.lastError().text();
        return "";
    }

    int userId = query.value(0).toInt();
    QString sessionId = QUuid::createUuid().toString(QUuid::WithoutBraces); // Уникальный токен

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO sessions (userid) VALUES (?)");
    insertQuery.addBindValue(userId);

    if (!insertQuery.exec()) {
        qDebug() << "Не удалось создать сессию:" << insertQuery.lastError().text();
        return "";
    }

    return sessionId;
}


bool Database::isValidSession(const QString &sessionId, QJsonObject &userData)
{
    QSqlQuery query;
    query.prepare("SELECT user.id, user.name, user.surname, user.role FROM sessions "
                  "JOIN user ON sessions.userid = user.id "
                  "WHERE sessions.id = ?");
    query.addBindValue(sessionId.toInt());

    if (!query.exec() || !query.next()) {
        qDebug() << "Сессия недействительна:" << query.lastError().text();
        return false;
    }

    userData["id"] = query.value("id").toInt();
    userData["name"] = query.value("name").toString();
    userData["surname"] = query.value("surname").toString();
    userData["role"] = query.value("role").toString();

    return true;
}

QString Database::removeSessionFromDB(const QString &token) {
    QSqlQuery query;

    query.prepare("SELECT COUNT(*) FROM sessions WHERE id = ?");
    query.addBindValue(token);
    if (!query.exec() || !query.next()) {
        qDebug() << "Не удалось найти сессию:" << query.lastError().text();
        return "error";
    }

    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM sessions (id) VALUES (?)");
    removeQuery.addBindValue(token);

    if (!removeQuery.exec()) {
        qDebug() << "Не удалось удалить сессию:" << removeQuery.lastError().text();
        return "error";
    }

    return "success";
}

bool Database::authenticateUser(const QString &login, const QString &password, QJsonObject &userData)
{
    if (!db.isOpen()) {
        qDebug() << "База данных не открыта";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT password, surname, name, role FROM user WHERE login = ?");
    query.addBindValue(login);

    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QString storedHashedPassword = query.value(0).toString();
        QString inputHashedPassword = hashPassword(password);

        if (storedHashedPassword == inputHashedPassword) {
            qDebug() << "Успешный вход";
            userData["surname"] = query.value("surname").toString();
            userData["name"] = query.value("name").toString();
            userData["role"] = query.value("role").toString();
            return true;
        } else {
            qDebug() << "Неверный пароль";
            return false;
        }
    }

    return false;
}

bool Database::addNewUser(const QString &login, const QString &password, const QString &surname, const QString &name)
{
    if (!db.isOpen()) {
        qDebug() << "База данных не открыта";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT id FROM user WHERE login = ?");
    query.addBindValue(login);

    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса, возможно пользователь уже существует:" << query.lastError().text();
        return false;
    }

    query.prepare("INSERT INTO user (name, surname, login, password, role) "
                  "VALUES (:name, :surname, :login, :password, :role)");
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.bindValue(":login", login);
    query.bindValue(":password", hashPassword(password));
    query.bindValue(":role", "student");

    if (!query.exec()) {
        qDebug() << "Failed to insert new user: " << query.lastError().text();
        return false;
    }

    return true;
}
