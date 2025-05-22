#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private:
    Ui::AdminWindow *ui;
    QLineEdit *searchLineEdit;
    QScrollArea *scrollArea;
    QWidget *scrollContent;
    QVBoxLayout *scrollLayout;

private slots:
    void searchUsers();
    void goView();
};

#endif // ADMINWINDOW_H
