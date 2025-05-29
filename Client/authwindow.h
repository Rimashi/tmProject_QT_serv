#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QWidget>
#include <QLineEdit>

class AuthWindow : public QWidget {
    Q_OBJECT

public:
    AuthWindow(QWidget *parent = nullptr);

private slots:
    void openRegisterWindow();
    void logSystem();

private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
};

#endif // AUTHWINDOW_H
