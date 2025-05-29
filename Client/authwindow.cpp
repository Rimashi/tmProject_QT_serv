#include "authwindow.h"
#include "mainwindow.h"
#include "adminwindow.h"
#include <QMessageBox>
#include "registerwindow.h"
#include "func2client.h"

AuthWindow::AuthWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Вход в систему");
    setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *titleLabel = new QLabel("Авторизация", this);
    titleLabel->setStyleSheet("font-size: 14pt; border-bottom: 0px solid #f4f4f4");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Логин");
    layout->addWidget(usernameInput);

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Пароль");
    passwordInput->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordInput);

    QPushButton *loginButton = new QPushButton("Войти", this);
    layout->addWidget(loginButton);

    QPushButton *registerButton = new QPushButton("Регистрация", this);
    layout->addWidget(registerButton);

    setStyleSheet("QWidget { background-color: #f4f4f4; color: black; }"
                  "QPushButton { background-color: black; color: white; border-radius: 5px; padding: 10px; }"
                  "QPushButton:hover { background-color: gray; }"
                  "QLineEdit { border: 2px solid black; border-radius: 5px; padding: 5px; }");

    setLayout(layout);

    connect(registerButton, &QPushButton::clicked, this, &AuthWindow::openRegisterWindow);
    connect(loginButton, &QPushButton::clicked, this, &AuthWindow::logSystem);
}

void AuthWindow::openRegisterWindow() {
    RegisterWindow *registerWindow = new RegisterWindow();
    registerWindow->show();
    this->close();
}

void AuthWindow::logSystem() {
    QString login = usernameInput->text();
    QString password = passwordInput->text();

    QString response = func2client::loginUser(login, password);
    if (response == "admin") {
        AdminWindow *adminWindow = new AdminWindow();
        adminWindow->show();
        this->close();
    }
    else if (response == "user") {
        MainWindow *mainWindow = new MainWindow();
        mainWindow->show();
        this->close();
    }
    else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
}
