#ifndef FUNC2CLIENT_H
#define FUNC2CLIENT_H

#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include "singleton_client.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <QEventLoop>
#include <QJsonObject>
#include <QMessageBox>
#include <QStatusBar>
#include <QSettings>
#include <QApplication>
#include <QTimer>
#include <cstring>

class func2client {
public:
    static QString registerUser(const QString& username, const QString& password);
    static QString loginUser(const QString& username, const QString& password);
    static QString getStatistics();
};

#endif // FUNC2CLIENT_H
