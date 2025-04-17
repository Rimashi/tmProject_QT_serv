#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "authwidget.h"
#include "networkclient.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLoginSuccess(const QJsonObject &response);
    void handleLoginFailed(const QString &error);
    void handleRegisterSuccess();
    void handleRegisterFailed(const QString &error);
    void toggleTheme(bool dark);

private:
    AuthWidget *authWidget;
    NetworkClient *networkClient;
};

#endif // MAINWINDOW_H
