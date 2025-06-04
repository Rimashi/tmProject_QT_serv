// database.h
#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QString>

class Database : public QObject {
    Q_OBJECT
public:
    static Database& instance() {
        static Database instance;
        return instance;
    }

    bool open(const QString& path);
    void close();

    bool userExists(const QString& login);
    bool registerUser(const QString& login, const QString& passwordHash, const QString& name, const QString& surname);
    bool validateUser(const QString& login, const QString& passwordHash, QJsonObject &userData); // добавим

    void initializeAdmin();

    QString lastError() const;

private:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    QSqlDatabase m_db;
    QString m_lastError;

    // запрещаем копирование
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
};

#endif // DATABASE_H
