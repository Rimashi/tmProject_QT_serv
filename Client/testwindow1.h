#ifndef TESTWINDOW1_H
#define TESTWINDOW1_H

#include <QWidget>

namespace Ui {
class TestWindow1;
}

class TestWindow1 : public QWidget
{
    Q_OBJECT

public:
    explicit TestWindow1(QWidget *parent = nullptr);
    ~TestWindow1();

private slots:
    void goMainTest1();

private:
    Ui::TestWindow1 *ui;
};

#endif // TESTWINDOW1_H
