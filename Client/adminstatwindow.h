#ifndef ADMINSTATWINDOW_H
#define ADMINSTATWINDOW_H

#include <QWidget>

namespace Ui {
class AdminStatWindow;
}

class AdminStatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminStatWindow(QWidget *parent = nullptr);
    ~AdminStatWindow();

private:
    Ui::AdminStatWindow *ui;

private slots:
    void showUsersWindow();

};

#endif // ADMINSTATWINDOW_H
