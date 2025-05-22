#ifndef STATISTICWINDOW_H
#define STATISTICWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
namespace Ui {
class StatisticWindow;
}

class StatisticWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticWindow(QWidget *parent = nullptr);
    ~StatisticWindow();
private slots:
    void goMainStat();
private:
    Ui::StatisticWindow *ui;
};

#endif // STATISTICWINDOW_H
