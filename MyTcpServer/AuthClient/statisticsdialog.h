#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QTableWidget>

class StatisticsDialog : public QDialog {
    Q_OBJECT
public:
    explicit StatisticsDialog(const QJsonObject& data, QWidget* parent = nullptr);

private:
    void setupUI(const QJsonObject& data);
    QTableWidget* table;
};

#endif // STATISTICSDIALOG_H
