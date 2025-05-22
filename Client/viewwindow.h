#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class ViewWindow;
}

struct TestResult {
    int testNumber;
    int score;
};

class ViewWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ViewWindow(QWidget *parent = nullptr);
    ~ViewWindow();
    void setTestResults(const QVector<TestResult> &results);

private:
    Ui::ViewWindow *ui;
    QTableWidget *tableWidget;
};

#endif // VIEWWINDOW_H
