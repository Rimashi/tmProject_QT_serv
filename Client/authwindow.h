#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "func2client.h"

// extern bool isDarkTheme;

class AuthWindow : public QWidget {
    Q_OBJECT
public:
    AuthWindow(QWidget *parent = nullptr);
private slots:
    void openRegisterWindow();
    void toggleTheme(bool dark);
    void logSystem();
private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput; 
};

#endif
