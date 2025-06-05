#include "windowmanager.h"
#include "authwidget.h"
#include "studentmainscreen.h"
#include "adminusersscreen.h"
#include "adminstatisticsscreen.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QMessageBox>

WindowManager::WindowManager(ClientAPI* api, QMainWindow* parent)
    : QObject(parent), mainWindow(parent), clientAPI(api)
{
    // Убедимся, что parent действительно является QMainWindow
    Q_ASSERT(mainWindow != nullptr);
}

void WindowManager::showAuthScreen() {
    if (currentScreen) {
        currentScreen->deleteLater();
        currentScreen = nullptr;
    }

    AuthWidget* authWidget = new AuthWidget(mainWindow);
    currentScreen = authWidget;
    mainWindow->setCentralWidget(currentScreen);

    // Подключаем сигналы
    connect(authWidget, &AuthWidget::loginRequested, [this](const QString& login, const QString& password) {
        if (!clientAPI->isConnected()) {
            QMessageBox::warning(mainWindow, "Ошибка", "Нет соединения с сервером");
            return;
        }
        clientAPI->sendLogin(login, password);
    });

    connect(authWidget, &AuthWidget::registerRequested, [this](
                                                            const QString& surname, const QString& name,
                                                            const QString& login, const QString& password) {
        if (!clientAPI->isConnected()) {
            QMessageBox::warning(mainWindow, "Ошибка", "Нет соединения с сервером");
            return;
        }
        clientAPI->sendRegister(surname, name, login, password);
    });

    qDebug() << "WindowManager: Showing auth screen";
}

void WindowManager::showMainScreenForAdmin() {
    if (currentScreen) {
        currentScreen->deleteLater();
        currentScreen = nullptr;
    }

    AdminUsersScreen* adminScreen = new AdminUsersScreen(clientAPI, mainWindow);
    currentScreen = adminScreen;
    mainWindow->setCentralWidget(currentScreen);

    connect(adminScreen, &AdminUsersScreen::showStatisticsRequested, [this]() {
        if (currentScreen) {
            currentScreen->deleteLater();
            currentScreen = nullptr;
        }
        AdminStatisticsScreen* statsScreen = new AdminStatisticsScreen(clientAPI, mainWindow);
        currentScreen = statsScreen;
        mainWindow->setCentralWidget(currentScreen);

        connect(statsScreen, &AdminStatisticsScreen::showUsersRequested, this, [this]() {
            showMainScreenForAdmin();
        });
        connect(statsScreen, &AdminStatisticsScreen::exitRequested, this, &WindowManager::exitRequested);
    });

    connect(adminScreen, &AdminUsersScreen::exitRequested, this, &WindowManager::exitRequested);
    connect(adminScreen, &AdminUsersScreen::showUsersRequested, this, [this]() {
        showMainScreenForAdmin();
    });
}

void WindowManager::showMainScreen(const QJsonObject& userData) {
    if (currentScreen) {
        currentScreen->deleteLater();
        currentScreen = nullptr;
    }

    QString role = userData["role"].toString();
    if (role == "student") {
        QString login = userData["login"].toString();
        qDebug() << "Student login:" << login;

        StudentMainScreen* studentScreen = new StudentMainScreen(login, clientAPI, mainWindow);
        currentScreen = studentScreen;
        mainWindow->setCentralWidget(currentScreen);

        connect(studentScreen, &StudentMainScreen::exitRequested,
                this, &WindowManager::exitRequested);

        qDebug() << "WindowManager: Showing main screen for student" << login;
    }else if (role == "admin") {
        // По умолчанию показываем экран пользователей
        AdminUsersScreen* adminScreen = new AdminUsersScreen(clientAPI, mainWindow);
        currentScreen = adminScreen;
        mainWindow->setCentralWidget(currentScreen);

        // Подключаем переходы
        connect(adminScreen, &AdminUsersScreen::showStatisticsRequested, [this]() {
            if (currentScreen) {
                currentScreen->deleteLater();
                currentScreen = nullptr;
            }
            AdminStatisticsScreen* statsScreen = new AdminStatisticsScreen(clientAPI, mainWindow);
            currentScreen = statsScreen;
            mainWindow->setCentralWidget(currentScreen);

            // Подключим переходы обратно
            connect(statsScreen, &AdminStatisticsScreen::showUsersRequested, this, [this]() {
                showMainScreenForAdmin();
            });
            connect(statsScreen, &AdminStatisticsScreen::exitRequested, this, &WindowManager::exitRequested);
        });

        connect(adminScreen, &AdminUsersScreen::exitRequested, this, &WindowManager::exitRequested);
        connect(adminScreen, &AdminUsersScreen::showUsersRequested, this, [this]() {
            showMainScreenForAdmin();
        });

    }else {
        qWarning() << "Unknown role:" << role;
        // Вернуться на экран авторизации или показать сообщение об ошибке
        showAuthScreen();
    }
}
