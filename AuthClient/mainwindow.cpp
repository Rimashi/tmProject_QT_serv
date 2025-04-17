#include "mainwindow.h"
#include <QMessageBox>
#include <QStatusBar>
#include <QSettings>
#include <QApplication>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Система авторизации");
    resize(400, 500);

    authWidget = new AuthWidget(this);
    networkClient = new NetworkClient(this);
    setCentralWidget(authWidget);

    networkClient->connectToServer("127.0.0.1", 55555);

    QTimer::singleShot(5000, [this]() {
        if (!networkClient->isConnected()) {
            QMessageBox::warning(this, "Ошибка", "Не удалось подключиться к серверу");
        }
    });

    connect(authWidget, &AuthWidget::loginRequested,
            networkClient, &NetworkClient::sendLogin);
    connect(authWidget, &AuthWidget::registerRequested,
            networkClient, &NetworkClient::sendRegister);
    connect(authWidget, &AuthWidget::themeToggled,
            this, &MainWindow::toggleTheme);

    connect(networkClient, &NetworkClient::loginSuccess,
            this, &MainWindow::handleLoginSuccess);
    connect(networkClient, &NetworkClient::loginFailed,
            this, &MainWindow::handleLoginFailed);
    connect(networkClient, &NetworkClient::registerSuccess,
            this, &MainWindow::handleRegisterSuccess);
    connect(networkClient, &NetworkClient::registerFailed,
            this, &MainWindow::handleRegisterFailed);
    connect(networkClient, &NetworkClient::errorOccurred, [this](const QString &error) {
        QMessageBox::critical(this, "Ошибка сети", error);
    });

    QSettings settings;
    toggleTheme(settings.value("darkTheme", false).toBool());
}

MainWindow::~MainWindow()
{
    networkClient->disconnectFromServer();
    delete authWidget;
    delete networkClient;
}

void MainWindow::handleLoginSuccess(const QJsonObject &response)
{
    QString role = response.value("role").toString("user");
    statusBar()->showMessage(QString("Вход выполнен. Роль: %1").arg(role), 5000);
    QMessageBox::information(this, "Успех", QString("Вы вошли как %1").arg(role));
}

void MainWindow::handleLoginFailed(const QString &error)
{
    QMessageBox::warning(this, "Ошибка входа", error);
}

void MainWindow::handleRegisterSuccess()
{
    statusBar()->showMessage("Регистрация успешна", 5000);
    authWidget->switchToLogin();
    QMessageBox::information(this, "Успех", "Регистрация завершена. Теперь вы можете войти.");
}

void MainWindow::handleRegisterFailed(const QString &error)
{
    QMessageBox::warning(this, "Ошибка регистрации", error);
}

void MainWindow::toggleTheme(bool dark)
{
    QSettings settings;
    settings.setValue("darkTheme", dark);
    authWidget->toggleTheme(dark);
}
