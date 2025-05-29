#ifndef FUNC2CLIENT_H
#define FUNC2CLIENT_H

#include <QString>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>

class func2client : public QObject{
    Q_OBJECT
public:
    static QString registerUser(const QString& surname, const QString& name, const QString& username, const QString& password);
    static QString loginUser(const QString& username, const QString& password);
    static QString getStatistics();
};

#endif // FUNC2CLIENT_H
