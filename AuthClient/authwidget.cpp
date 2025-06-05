#include "authwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

AuthWidget::AuthWidget(QWidget *parent) : QWidget(parent)
{
    // Установка фиксированного размера 800x600
    this->setFixedSize(800, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(20);

    stack = new QStackedWidget(this);
    setupLoginForm();
    setupRegisterForm();
    mainLayout->addWidget(stack);

    setLayout(mainLayout);
    toggleTheme(false);
}

void AuthWidget::setupLoginForm()
{
    QWidget *loginWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(loginWidget);
    layout->setContentsMargins(40, 20, 40, 30);
    layout->setSpacing(25);

    QLabel *title = new QLabel("ВХОД", loginWidget);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size: 34px; "
        "font-weight: bold; "
        "margin: 4px; "
        "padding: 4px; "
        );
    layout->addWidget(title);

    QLineEdit *loginEdit = new QLineEdit(loginWidget);
    loginEdit->setPlaceholderText("Логин");
    loginEdit->setMinimumHeight(60);  // Увеличенная высота
    loginEdit->setStyleSheet("font-size: 18px;");
    layout->addWidget(loginEdit);

    QLineEdit *passEdit = new QLineEdit(loginWidget);
    passEdit->setPlaceholderText("Пароль");
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setMinimumHeight(60);  // Увеличенная высота
    passEdit->setStyleSheet("font-size: 18px;");
    layout->addWidget(passEdit);

    connect(passEdit, &QLineEdit::returnPressed, [this, loginEdit, passEdit]() {
        if (!loginEdit->text().isEmpty() && !passEdit->text().isEmpty()) {
            emit loginRequested(loginEdit->text(), passEdit->text());
        }
    });

    QHBoxLayout *linkLayout = new QHBoxLayout();
    linkLayout->addStretch();
    QPushButton *regLink = new QPushButton("Еще нет аккаунта?", loginWidget);
    regLink->setFlat(true);
    regLink->setStyleSheet("font-size: 16px; text-align: center;");
    linkLayout->addWidget(regLink);
    linkLayout->addStretch();
    layout->addLayout(linkLayout);

    QPushButton *loginBtn = new QPushButton("Войти", loginWidget);
    loginBtn->setMinimumHeight(60);  // Увеличенная высота
    loginBtn->setStyleSheet(
        "font-size: 20px; font-weight: bold;"
        "background-color: black; color: white;"
        "border-radius: 6px;"
        );
    connect(loginBtn, &QPushButton::clicked, [this, loginEdit, passEdit]() {
        if (loginEdit->text().isEmpty() || passEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Заполните все поля");
            return;
        }
        emit loginRequested(loginEdit->text(), passEdit->text());
    });
    layout->addWidget(loginBtn);

    connect(regLink, &QPushButton::clicked, this, &AuthWidget::switchToRegister);
    stack->addWidget(loginWidget);
}

void AuthWidget::setupRegisterForm()
{
    QWidget *regWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(regWidget);
    layout->setContentsMargins(40, 20, 40, 30);  // Такие же отступы как в login
    layout->setSpacing(20);

    QLabel *title = new QLabel("РЕГИСТРАЦИЯ", regWidget);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size: 34px; "
        "font-weight: bold; "
        "margin: 0; "
        "padding: 0; "
        );
    layout->addWidget(title);

    QLineEdit *surnameEdit = new QLineEdit(regWidget);
    surnameEdit->setPlaceholderText("Фамилия");
    surnameEdit->setMinimumHeight(60);
    surnameEdit->setStyleSheet("font-size: 18px;");
    layout->addWidget(surnameEdit);

    QLineEdit *nameEdit = new QLineEdit(regWidget);
    nameEdit->setPlaceholderText("Имя");
    nameEdit->setMinimumHeight(60);
    nameEdit->setStyleSheet("font-size: 18px;");
    layout->addWidget(nameEdit);

    QLineEdit *loginEdit = new QLineEdit(regWidget);
    loginEdit->setPlaceholderText("Логин");
    loginEdit->setMinimumHeight(60);
    loginEdit->setStyleSheet("font-size: 18px;");
    layout->addWidget(loginEdit);

    QLineEdit *passEdit = new QLineEdit(regWidget);
    passEdit->setPlaceholderText("Пароль");
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setMinimumHeight(60);
    passEdit->setStyleSheet("font-size: 18px;");
    layout->addWidget(passEdit);

    connect(passEdit, &QLineEdit::returnPressed, [this, surnameEdit, nameEdit, loginEdit, passEdit]() {
        if (!surnameEdit->text().isEmpty() && !nameEdit->text().isEmpty() &&
            !loginEdit->text().isEmpty() && !passEdit->text().isEmpty()) {
            emit registerRequested(surnameEdit->text(), nameEdit->text(),
                                   loginEdit->text(), passEdit->text());
        }
    });

    QHBoxLayout *linkLayout = new QHBoxLayout();
    linkLayout->addStretch();
    QPushButton *loginLink = new QPushButton("Уже есть аккаунт?", regWidget);
    loginLink->setFlat(true);
    loginLink->setStyleSheet("font-size: 16px; text-align: center;");
    linkLayout->addWidget(loginLink);
    linkLayout->addStretch();
    layout->addLayout(linkLayout);

    QPushButton *regBtn = new QPushButton("Зарегистрироваться", regWidget);
    regBtn->setMinimumHeight(60);
    regBtn->setStyleSheet(
        "font-size: 20px; font-weight: bold;"
        "background-color: black; color: white;"
        "border-radius: 6px;"
        );
    connect(regBtn, &QPushButton::clicked, [this, surnameEdit, nameEdit, loginEdit, passEdit]() {
        if (surnameEdit->text().isEmpty() || nameEdit->text().isEmpty() ||
            loginEdit->text().isEmpty() || passEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Заполните все поля");
            return;
        }
        emit registerRequested(surnameEdit->text(), nameEdit->text(),
                               loginEdit->text(), passEdit->text());
    });
    layout->addWidget(regBtn);

    connect(loginLink, &QPushButton::clicked, this, &AuthWidget::switchToLogin);
    stack->addWidget(regWidget);
}

void AuthWidget::toggleTheme(bool dark)
{
    // Теперь используем только светлую тему
    QString style =
        "QWidget { "
        "    background: #ffffff; "
        "    color: #000000; "
        "}"
        "QLineEdit { "
        "    background: #ffffff; "
        "    color: #000000; "
        "    border: 2px solid #cccccc; "
        "    padding: 15px; "
        "    border-radius: 6px; "
        "}"
        "QPushButton { "
        "    background: #f0f0f0; "
        "    color: #000000; "
        "    border: 2px solid #dddddd; "
        "    padding: 15px; "
        "    border-radius: 6px; "
        "    min-width: 200px; "
        "}"
        "QPushButton:hover { "
        "    background: #e0e0e0; "
        "}"
        "QPushButton:flat { "
        "    background: transparent; "
        "    color: #555555; "
        "    border: none; "
        "    padding: 0; "
        "    min-width: 0; "
        "}";

    setStyleSheet(style);
}
