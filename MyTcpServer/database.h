#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QJsonObject>

class Database : public QObject
{
    Q_OBJECT

public:
    static Database* instance(); // главный метод для доступа к экземпляру
    ~Database();

    bool initDatabase();
    QString createSessionForUser(const QString &login);
    QString removeSessionFromDB(const QString &token);
    bool isValidSession(const QString &sessionId, QJsonObject &userData);
    bool authenticateUser(const QString &login, const QString &password, QJsonObject &userData);
    bool addNewUser(const QString &login, const QString &password, const QString &surname, const QString &name);

private:
    explicit Database(QObject *parent = nullptr); // закрытый конструктор
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    bool checkTableExists(const QString &tableName);
    bool createTables();
    bool addAdminIfNeeded();

    QSqlDatabase db;
};

#endif // DATABASE_H
