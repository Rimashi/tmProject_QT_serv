#ifndef TEST1DIALOG_H
#define TEST1DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "client_api.h"

class Test1Dialog : public QDialog {
    Q_OBJECT
public:
    explicit Test1Dialog(const QString& task, const QString& login, ClientAPI* clientAPI, QWidget* parent = nullptr);

private slots:
    void onSubmit();
    void onHideTask();  // Добавляем слот для обработки скрытия/показа задачи

private:
    QString studentLogin;
    QString currentTask;
    ClientAPI* api;

    QLabel* taskLabel;
    QLineEdit* answerInput;
    QPushButton* hideBtn;  // Добавляем как член класса
    bool isTaskHidden = false;
};

#endif // TEST1DIALOG_H
