#ifndef ADMINUSERSCREEN_H
#define ADMINUSERSCREEN_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "client_api.h"

class AdminUsersScreen : public QWidget
{
    Q_OBJECT
public:
    explicit AdminUsersScreen(ClientAPI* api, QWidget *parent = nullptr);

signals:
    void showStatisticsRequested();
    void showUsersRequested();
    void exitRequested();

private slots:
    void onSearchClicked();
    void onUserSelected(QListWidgetItem* item);
    void handleUsersListReceived(const QJsonArray& users);
    void handleUserStatisticsReceived(const QJsonObject& userData);

private:
    void setupUI();

    ClientAPI* api;
    QListWidget* usersList;
};

#endif // ADMINUSERSCREEN_H
