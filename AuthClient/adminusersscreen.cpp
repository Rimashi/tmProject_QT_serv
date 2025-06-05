#include "adminusersscreen.h"
#include <QJsonObject>
#include <QJsonArray>
#include "userstatisticsdialog.h"

AdminUsersScreen::AdminUsersScreen(ClientAPI* api, QWidget *parent)
    : QWidget(parent), api(api)
{
    setupUI();
    connect(api, &ClientAPI::usersListReceived, this, &AdminUsersScreen::handleUsersListReceived);
    connect(api, &ClientAPI::userStatisticsReceived, this, &AdminUsersScreen::handleUserStatisticsReceived);
    api->getUsersAdmin();
}

void AdminUsersScreen::setupUI() {
    // Основной стиль
    this->setStyleSheet("QWidget { background-color: #ffffff; }");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    // Заголовок
    QLabel* titleLabel = new QLabel("АДМИНИСТРАТОР", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 24pt;"
        "font-weight: bold;"
        "color: #000000;"
        "margin-bottom: 20px;"
        "border-bottom: 2px solid #000000;"
        "padding-bottom: 10px;"
        );
    layout->addWidget(titleLabel);

    // Панель поиска
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->setContentsMargins(0, 0, 0, 10);

    QLineEdit* searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Поиск по имени...");
    searchEdit->setStyleSheet(
        "QLineEdit {"
        "   border: 1px solid #000000;"
        "   padding: 8px;"
        "   font-size: 12pt;"
        "   min-width: 300px;"
        "}"
        );

    QPushButton* searchBtn = new QPushButton("НАЙТИ", this);
    searchBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #000000;"
        "   color: #ffffff;"
        "   border: none;"
        "   padding: 8px 20px;"
        "   font-size: 12pt;"
        "}"
        "QPushButton:hover {"
        "   background-color: #333333;"
        "}"
        );

    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchBtn);
    layout->addLayout(searchLayout);

    // Список пользователей
    usersList = new QListWidget(this);
    usersList->setStyleSheet(
        "QListWidget {"
        "   border: 1px solid #000000;"
        "   font-size: 12pt;"
        "}"
        "QListWidget::item {"
        "   padding: 8px;"
        "   border-bottom: 1px solid #e0e0e0;"
        "}"
        "QListWidget::item:hover {"
        "   background-color: #f0f0f0;"
        "}"
        );
    layout->addWidget(usersList);

    // Кнопки навигации
    QHBoxLayout* navLayout = new QHBoxLayout();
    navLayout->setSpacing(10);

    QPushButton* btnUsers = new QPushButton("ПОЛЬЗОВАТЕЛИ", this);
    QPushButton* btnStats = new QPushButton("СТАТИСТИКА", this);
    QPushButton* btnExit = new QPushButton("ВЫХОД", this);

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #ffffff;"
        "   color: #000000;"
        "   border: 1px solid #000000;"
        "   padding: 8px 15px;"
        "   font-size: 11pt;"
        "}"
        "QPushButton:hover {"
        "   background-color: #000000;"
        "   color: #ffffff;"
        "}";

    btnUsers->setStyleSheet(buttonStyle);
    btnStats->setStyleSheet(buttonStyle);
    btnExit->setStyleSheet(buttonStyle);

    navLayout->addWidget(btnUsers);
    navLayout->addWidget(btnStats);
    navLayout->addWidget(btnExit);
    layout->addLayout(navLayout);

    // Подключение сигналов
    connect(searchBtn, &QPushButton::clicked, this, &AdminUsersScreen::onSearchClicked);
    connect(btnStats, &QPushButton::clicked, this, &AdminUsersScreen::showStatisticsRequested);
    connect(btnExit, &QPushButton::clicked, this, &AdminUsersScreen::exitRequested);
    connect(btnUsers, &QPushButton::clicked, this, &AdminUsersScreen::showUsersRequested);
    connect(usersList, &QListWidget::itemDoubleClicked, this, &AdminUsersScreen::onUserSelected);
}
void AdminUsersScreen::onSearchClicked() {
    api->getUsersAdmin();
}
void AdminUsersScreen::onUserSelected(QListWidgetItem* item) {
    QString login = item->data(Qt::UserRole).toString();
    api->getUserInfo(login);
}

void AdminUsersScreen::handleUsersListReceived(const QJsonArray& users) {
    usersList->clear();
    for (const QJsonValue& userVal : users) {
        QJsonObject user = userVal.toObject();
        QString login = user["login"].toString();
        QString name = user["name"].toString();
        QString surname = user["surname"].toString();
        QString displayText = surname + " " + name + " (" + login + ")";

        QListWidgetItem* item = new QListWidgetItem(displayText, usersList);
        item->setData(Qt::UserRole, login);
    }
}

void AdminUsersScreen::handleUserStatisticsReceived(const QJsonObject& userData) {
    if (userData.contains("surname") && userData.contains("name") && userData.contains("statistics")) {
        UserStatisticsDialog* dialog = new UserStatisticsDialog(userData, this);
        dialog->exec();
        dialog->deleteLater();
    }
}
