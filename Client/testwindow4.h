#ifndef TESTWINDOW4_H
#define TESTWINDOW4_H

#include <QWidget>

namespace Ui {
class TestWindow4;
}

class TestWindow4 : public QWidget
{
    Q_OBJECT

public:
    explicit TestWindow4(QWidget *parent = nullptr);
    ~TestWindow4();

private slots:
    void goMainTest4();

private:
    Ui::TestWindow4 *ui;
};

#endif // TESTWINDOW4_H
