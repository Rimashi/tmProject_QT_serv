#ifndef TESTWINDOW3_H
#define TESTWINDOW3_H

#include <QWidget>

namespace Ui {
class TestWindow3;
}

class TestWindow3 : public QWidget
{
    Q_OBJECT

public:
    explicit TestWindow3(QWidget *parent = nullptr);
    ~TestWindow3();

private slots:
    void goMainTest3();

private:
    Ui::TestWindow3 *ui;
};

#endif // TESTWINDOW3_H
