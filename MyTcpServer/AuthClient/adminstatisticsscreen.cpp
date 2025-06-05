#include "adminstatisticsscreen.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <QTableWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDebug>

AdminStatisticsScreen::AdminStatisticsScreen(ClientAPI* api, QWidget *parent)
    : QWidget(parent), api(api)
{
    setupUI();
    connect(api, &ClientAPI::adminStatisticsReceived, this, &AdminStatisticsScreen::handleAdminStatisticsReceived);
    api->getStatisticsAdmin();
}

void AdminStatisticsScreen::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Заголовок
    QLabel* titleLabel = new QLabel("Администратор - Статистика", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18pt; font-weight: bold; margin-bottom: 15px;");
    layout->addWidget(titleLabel);

    // Поисковая строка
    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLabel* searchLabel = new QLabel("Поиск по фамилии:", this);
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("Введите фамилию...");
    QPushButton* searchButton = new QPushButton("Найти", this);

    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchLineEdit);
    searchLayout->addWidget(searchButton);
    layout->addLayout(searchLayout);

    // Таблица статистики
    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels(QStringList() << "Фамилия" << "Имя" << "№ теста" << "Дата" << "Результат");
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    layout->addWidget(table);

    // Кнопки навигации
    QHBoxLayout* navLayout = new QHBoxLayout();
    QPushButton* btnUsers = new QPushButton("Пользователи", this);
    QPushButton* btnStats = new QPushButton("Статистика", this);
    QPushButton* btnExit = new QPushButton("Выход", this);
    navLayout->addWidget(btnUsers);
    navLayout->addWidget(btnStats);
    navLayout->addWidget(btnExit);
    layout->addLayout(navLayout);

    // Подключение сигналов
    connect(btnUsers, &QPushButton::clicked, this, &AdminStatisticsScreen::showUsersRequested);
    connect(btnStats, &QPushButton::clicked, this, &AdminStatisticsScreen::showStatisticsRequested);
    connect(btnExit, &QPushButton::clicked, this, &AdminStatisticsScreen::exitRequested);
    connect(searchButton, &QPushButton::clicked, this, &AdminStatisticsScreen::filterTable);
    connect(searchLineEdit, &QLineEdit::textChanged, this, &AdminStatisticsScreen::filterTable);
}

void AdminStatisticsScreen::handleAdminStatisticsReceived(const QJsonArray& stats) {
    allStats = stats; // Сохраняем все данные для фильтрации
    updateTable(allStats);
}

void AdminStatisticsScreen::updateTable(const QJsonArray& stats) {
    table->setRowCount(stats.size());

    int row = 0;
    for (const QJsonValue& statVal : stats) {
        QJsonObject stat = statVal.toObject();

        QString surname = stat["surname"].toString();
        QString name = stat["name"].toString();
        QString testNumber = stat["test_number"].toString();
        QString isoDate = stat["date"].toString();
        QString score = stat["score"].toString();

        // Форматирование даты
        QDateTime dateTime = QDateTime::fromString(isoDate, Qt::ISODate);
        QString formattedDate = dateTime.toString("dd.MM.yyyy HH:mm");

        table->setItem(row, 0, new QTableWidgetItem(surname));
        table->setItem(row, 1, new QTableWidgetItem(name));
        table->setItem(row, 2, new QTableWidgetItem(testNumber));
        table->setItem(row, 3, new QTableWidgetItem(formattedDate));
        table->setItem(row, 4, new QTableWidgetItem(score));

        row++;
    }
}
void AdminStatisticsScreen::filterTable() {
    QString searchText = searchLineEdit->text().trimmed().toLower();

    if (searchText.isEmpty()) {
        updateTable(allStats);
        return;
    }

    QJsonArray filteredStats;
    for (const QJsonValue& statVal : allStats) {
        QJsonObject stat = statVal.toObject();
        QString surname = stat["surname"].toString().toLower();

        if (surname.contains(searchText)) {
            filteredStats.append(stat);
        }
    }

    updateTable(filteredStats);
}
