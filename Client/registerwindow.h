#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QLineEdit>

class RegisterWindow : public QWidget {
    Q_OBJECT

public:
    RegisterWindow(QWidget *parent = nullptr);

private slots:
    void goBack();
    void regTry();

private:
    QLineEdit *surnameInput;
    QLineEdit *nameInput;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
};

#endif // REGISTERWINDOW_H
