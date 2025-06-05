#include "mainwindow.h"
#include "func2client.h"
#include <QMessageBox>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Система управления");
    resize(400, 500);

    // Инициализация API
    clientAPI = new ClientAPI(new Func2Client(this), this);

    // Инициализация WindowManager с главным окном
    windowManager = new WindowManager(clientAPI, this);

    // Показываем экран авторизации при запуске
    windowManager->showAuthScreen();

    // Подключаемся к серверу
    clientAPI->connectToServer("127.0.0.1", 3000);

    // Таймер проверки соединения
    QTimer::singleShot(5000, [this] {
        if (!clientAPI->isConnected()) {
            QMessageBox::warning(this, "Ошибка", "Не удалось подключиться к серверу");
        }
    });

    // Подключаем сигналы API
    connect(clientAPI, &ClientAPI::loginSuccess, this, &MainWindow::handleLoginSuccess);
    connect(clientAPI, &ClientAPI::loginFailed, this, &MainWindow::handleLoginFailed);
    connect(clientAPI, &ClientAPI::registerSuccess, this, &MainWindow::handleRegisterSuccess);
    connect(clientAPI, &ClientAPI::registerFailed, this, &MainWindow::handleRegisterFailed);

    // Обработка выхода из системы
    connect(windowManager, &WindowManager::exitRequested, this, &MainWindow::handleExitRequested);
}

MainWindow::~MainWindow() {
    clientAPI->disconnectFromServer();
    delete windowManager;
    delete clientAPI;
}

void MainWindow::handleLoginSuccess(const QJsonObject& response) {
    QString name = response["name"].toString();
    QString surname = response["surname"].toString();

    qDebug() << "Login success:" << surname << name;

    // Переключаемся на главный экран
    windowManager->showMainScreen(response);
}

void MainWindow::handleLoginFailed(const QString& error) {
    QMessageBox::warning(this, "Ошибка входа", error);
}

void MainWindow::handleRegisterSuccess() {
    QMessageBox::information(this, "Успех", "Регистрация завершена. Теперь вы можете войти.");
}

void MainWindow::handleRegisterFailed(const QString& error) {
    QMessageBox::warning(this, "Ошибка регистрации", error);
}

void MainWindow::handleExitRequested() {
    clientAPI->disconnectFromServer();
    windowManager->showAuthScreen(); // Возвращаемся к экрану авторизации
}
