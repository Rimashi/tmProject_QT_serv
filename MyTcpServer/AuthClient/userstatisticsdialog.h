#ifndef USERSTATISTICSDIALOG_H
#define USERSTATISTICSDIALOG_H

#include <QDialog>
#include <QJsonObject>

// class QJsonObject;

class UserStatisticsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UserStatisticsDialog(const QJsonObject& userData, QWidget *parent = nullptr);

private:
    void setupUI(const QJsonObject& data);
};

#endif // USERSTATISTICSDIALOG_H
