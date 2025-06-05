#ifndef AUTHWIDGET_H
#define AUTHWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class AuthWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AuthWidget(QWidget *parent = nullptr);
    void toggleTheme(bool dark);
    void switchToLogin() { stack->setCurrentIndex(0); }
    void switchToRegister() { stack->setCurrentIndex(1); }

signals:
    void loginRequested(const QString &login, const QString &password);
    void registerRequested(const QString &surname, const QString &name,
                           const QString &login, const QString &password);
    void themeToggled(bool dark);

private:
    QStackedWidget *stack;
    bool isDarkTheme = false;

    void setupLoginForm();
    void setupRegisterForm();
};

#endif // AUTHWIDGET_H
