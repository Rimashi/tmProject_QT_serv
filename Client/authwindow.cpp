#include "authwindow.h"
#include "mainwindow.h"
#include "adminwindow.h"
#include <QMessageBox>
#include "registerwindow.h"
#include <QApplication>
#include "global.h"

// static bool isDarkTheme = false;

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

    setStyleSheet("QWidget { background-color: #f4f4f4; color: black;}"
                  "QPushButton { background-color: black;color: white; border-radius: 5px; padding: 10px;}"
                  "QPushButton:hover { background-color: gray;}"
                  "QLineEdit {border: 2px solid black; border-radius: 5px; padding: 5px;}"
                  );

    setLayout(layout);

    connect(registerButton, &QPushButton::clicked, this, &AuthWindow::openRegisterWindow);
    connect(loginButton, &QPushButton::clicked, this, &AuthWindow::logSystem);
    //connect(themeToggle, &QPushButton::clicked, this, &AuthWindow::toggleTheme);
}

void AuthWindow::openRegisterWindow() {
    RegisterWindow *registerWindow = new RegisterWindow();
    registerWindow->show();
    this->close();
}

void AuthWindow::toggleTheme(bool dark)
{
    isDarkTheme = dark;
    QString style = dark ?
                        "QWidget { background: #121212; color: white; }"
                        "QLineEdit { background: #333; color: white; border: 1px solid #555; padding: 8px; border-radius: 4px; }"
                        "QPushButton { background: #555; color: white; border: none; padding: 8px; border-radius: 4px; }"
                        "QPushButton:hover { background: #666; }"
                        "QPushButton:flat { background: transparent; color: #4da6ff; text-align: left; }" :
                        "QWidget { background: white; color: black; }"
                        "QLineEdit { background: white; color: black; border: 1px solid #ddd; padding: 8px; border-radius: 4px; }"
                        "QPushButton { background: #000; color: white; border: none; padding: 8px; border-radius: 4px; }"
                        "QPushButton:hover { background: #333; }"
                        "QPushButton:flat { background: transparent; color: #0066cc; text-align: left; }";
    setStyleSheet(style);
}

void AuthWindow::logSystem() {
    // Получаем введенные логин и пароль
    QString login = usernameInput->text();
    QString password = passwordInput->text();

    // Проверяем учетные данные администратора
    if (auth(login, password)=='a') {
        AdminWindow *adminWindow = new AdminWindow();
        adminWindow->show();
        this->close();
        //emit auth_user_ok();
    }
    // Проверяем учетные данные пользователя
    else if (auth(login, password)=='u') {
        MainWindow *mainWindow = new MainWindow();
        mainWindow->show();
        this->close();
    }
    // Неверные учетные данные
    else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
}


