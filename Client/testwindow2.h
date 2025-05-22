#ifndef TESTWINDOW2_H
#define TESTWINDOW2_H

#include <QWidget>

namespace Ui {
class TestWindow2;
}

class TestWindow2 : public QWidget
{
    Q_OBJECT

public:
    explicit TestWindow2(QWidget *parent = nullptr);
    ~TestWindow2();

private slots:
    void goMainTest2();

private:
    Ui::TestWindow2 *ui;
};

#endif // TESTWINDOW2_H
