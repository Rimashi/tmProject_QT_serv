#ifndef STUDENTMAINSCREEN_H
#define STUDENTMAINSCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "client_api.h"

class StudentMainScreen : public QWidget {
    Q_OBJECT
public:
    explicit StudentMainScreen(const QString& login, ClientAPI* clientAPI, QWidget* parent = nullptr);

signals:
    void exitRequested();  // Сигнал для выхода

private slots:
    void onStatisticsClicked();
    void onTest1Clicked();
    void onTest2Clicked();
    void onExitClicked();
    void handleStatisticsReceived(const QJsonObject& data);
    void handleTask1Received(const QString& task);
    void handleTask2Received(const QString& task);

private:
    QString studentLogin;
    ClientAPI* api;
    QPushButton* btnStatistics;
    QPushButton* btnTest1;
    QPushButton* btnTest2;
    QPushButton* btnTests;
    QPushButton* btnExit;
};

#endif // STUDENTMAINSCREEN_H
