#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "func2client.h"

class RegisterWindow : public QWidget {
    Q_OBJECT
public:
    RegisterWindow(QWidget *parent = nullptr);
private slots:
    void goBack();
    void toggleTheme();
    void regTry();
private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
};

#endif
