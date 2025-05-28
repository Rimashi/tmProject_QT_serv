#ifndef FUNC2CLIENT_H
#define FUNC2CLIENT_H
#include <QString>
#include "networkclient.h"
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

void handleLoginSuccess(const QJsonObject &response);
void handleLoginFailed(const QString &error);
void handleRegisterSuccess();
void handleRegisterFailed(const QString &error);
void toggleTheme(bool dark);

char auth(QString login, QString password);
bool reg(QString login, QString password);
std::vector<uint8_t> pad_message(const std::string& message);
#endif // FUNC2CLIENT_H
