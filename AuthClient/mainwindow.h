#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client_api.h"
#include "windowmanager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void handleLoginSuccess(const QJsonObject& userData);
    void handleLoginFailed(const QString& error);
    void handleRegisterSuccess();
    void handleRegisterFailed(const QString& error);
    void handleExitRequested();

private:
    ClientAPI* clientAPI;
    WindowManager* windowManager;
};
#endif // MAINWINDOW_H
