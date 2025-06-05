#ifndef ADMINSTATISTICSSCREEN_H
#define ADMINSTATISTICSSCREEN_H

#include <QWidget>
#include <QLineEdit>
#include <QJsonArray>
#include "client_api.h"

class QTableWidget;

class AdminStatisticsScreen : public QWidget
{
    Q_OBJECT
public:
    explicit AdminStatisticsScreen(ClientAPI* api, QWidget *parent = nullptr);

signals:
    void showStatisticsRequested();
    void showUsersRequested();
    void exitRequested();

private slots:
    void handleAdminStatisticsReceived(const QJsonArray& stats);

private:
    void setupUI();

    ClientAPI* api;
    QTableWidget* table;
    QJsonArray allStats;
    QLineEdit* searchLineEdit;

    void updateTable(const QJsonArray& stats);
    void filterTable();
};

#endif // ADMINSTATISTICSSCREEN_H
