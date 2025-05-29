#include "registerwindow.h"
#include "authwindow.h"
#include "mainwindow.h"
#include "func2client.h"
#include <QMessageBox>

RegisterWindow::RegisterWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Регистрация");
    setFixedSize(400, 350);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *titleLabel = new QLabel("Регистрация", this);
    titleLabel->setStyleSheet("font-size: 14pt; border-bottom: 0px solid #f4f4f4");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    surnameInput = new QLineEdit(this);
    surnameInput->setPlaceholderText("Фамилия");
    layout->addWidget(surnameInput);

    nameInput = new QLineEdit(this);
    nameInput->setPlaceholderText("Имя");
    layout->addWidget(nameInput);

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Логин");
    layout->addWidget(usernameInput);

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Пароль");
    passwordInput->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordInput);

    QPushButton *registerButton = new QPushButton("Зарегистрироваться", this);
    layout->addWidget(registerButton);

    QPushButton *backButton = new QPushButton("Назад", this);
    layout->addWidget(backButton);

    setStyleSheet("QWidget { background-color: #f4f4f4; color: black; }"
                  "QPushButton { background-color: black; color: white; border-radius: 5px; padding: 10px; }"
                  "QPushButton:hover { background-color: gray; }"
                  "QLineEdit { border: 2px solid black; border-radius: 5px; padding: 5px; }");

    setLayout(layout);

    connect(backButton, &QPushButton::clicked, this, &RegisterWindow::goBack);
    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::regTry);
}

void RegisterWindow::goBack() {
    AuthWindow *authWindow = new AuthWindow();
    authWindow->show();
    this->close();
}

void RegisterWindow::regTry() {
    QString login = usernameInput->text();
    QString password = passwordInput->text();

    QString response = func2client::registerUser(login, password);
    if (response == "ok") {
        QMessageBox::information(this, "Регистрация", "Регистрация успешна!");
        MainWindow *mainWindow = new MainWindow();
        mainWindow->show();
        this->close();
    }
    else {
        QMessageBox::warning(this, "Ошибка", "Регистрация не удалась. Возможно, такой логин уже существует.");
    }
}
