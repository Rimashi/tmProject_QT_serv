#include "authwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

AuthWidget::AuthWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QPushButton *themeToggle = new QPushButton("🌙 Dark Mode", this);
    themeToggle->setFixedSize(120, 30);
    connect(themeToggle, &QPushButton::clicked, [this, themeToggle]() {
        isDarkTheme = !isDarkTheme;
        themeToggle->setText(isDarkTheme ? "☀️ Light Mode" : "🌙 Dark Mode");
        emit themeToggled(isDarkTheme);
    });

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addStretch();
    topLayout->addWidget(themeToggle);
    mainLayout->addLayout(topLayout);

    stack = new QStackedWidget(this);
    setupLoginForm();
    setupRegisterForm();
    mainLayout->addWidget(stack);

    setLayout(mainLayout);
}

void AuthWidget::setupLoginForm()
{
    QWidget *loginWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(loginWidget);
    layout->setSpacing(15);

    QLabel *title = new QLabel("Вход", loginWidget);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(title);

    QLineEdit *loginEdit = new QLineEdit(loginWidget);
    loginEdit->setPlaceholderText("Логин");
    layout->addWidget(loginEdit);

    QLineEdit *passEdit = new QLineEdit(loginWidget);
    passEdit->setPlaceholderText("Пароль");
    passEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passEdit);

    connect(passEdit, &QLineEdit::returnPressed, [this, loginEdit, passEdit]() {
        if (!loginEdit->text().isEmpty() && !passEdit->text().isEmpty()) {
            emit loginRequested(loginEdit->text(), passEdit->text());
        }
    });

    QPushButton *regLink = new QPushButton("Еще нет аккаунта?", loginWidget);
    regLink->setFlat(true);
    layout->addWidget(regLink);

    QPushButton *loginBtn = new QPushButton("Войти", loginWidget);
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
    layout->setSpacing(15);

    QLabel *title = new QLabel("Регистрация", regWidget);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(title);

    QLineEdit *surnameEdit = new QLineEdit(regWidget);
    surnameEdit->setPlaceholderText("Фамилия");
    layout->addWidget(surnameEdit);

    QLineEdit *nameEdit = new QLineEdit(regWidget);
    nameEdit->setPlaceholderText("Имя");
    layout->addWidget(nameEdit);

    QLineEdit *loginEdit = new QLineEdit(regWidget);
    loginEdit->setPlaceholderText("Логин");
    layout->addWidget(loginEdit);

    QLineEdit *passEdit = new QLineEdit(regWidget);
    passEdit->setPlaceholderText("Пароль");
    passEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passEdit);

    connect(passEdit, &QLineEdit::returnPressed, [this, surnameEdit, nameEdit, loginEdit, passEdit]() {
        if (!surnameEdit->text().isEmpty() && !nameEdit->text().isEmpty() &&
            !loginEdit->text().isEmpty() && !passEdit->text().isEmpty()) {
            emit registerRequested(surnameEdit->text(), nameEdit->text(),
                                   loginEdit->text(), passEdit->text());
        }
    });

    QPushButton *loginLink = new QPushButton("Уже есть аккаунт?", regWidget);
    loginLink->setFlat(true);
    layout->addWidget(loginLink);

    QPushButton *regBtn = new QPushButton("Зарегистрироваться", regWidget);
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
