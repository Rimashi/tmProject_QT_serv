#include "adminstatwindow.h"
#include "adminwindow.h"
#include "ui_adminstatwindow.h"
#include <QApplication>
#include <QHeaderView>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QListWidget>
#include <QFrame>
#include <QLineEdit>
#include <QSpacerItem>

AdminStatWindow::AdminStatWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminStatWindow)
{
    ui->setupUi(this);
    resize(1000, 600);
    this->setFixedSize(1000, 600);
    setWindowTitle("Администратор - Статистика");
    setStyleSheet("background-color: white; color: black;");

    // Главный контейнер (горизонтальный)
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Левая панель (меню)
    QFrame *leftPanel = new QFrame();
    leftPanel->setFixedWidth(200);
    leftPanel->setStyleSheet("background-color: #f0f0f0; border-right: 1px solid #d0d0d0;");

    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(10, 20, 10, 20);
    leftLayout->setSpacing(15);

    // Кнопки меню
    QPushButton *usersButton = new QPushButton("Пользователи");
    usersButton->setFixedHeight(40);
    usersButton->setStyleSheet(
        "QPushButton {"
        "   background-color: black;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #d0d0d0;"
        "}"
        );

    QPushButton *statsButton = new QPushButton("Статистика");
    statsButton->setFixedHeight(40);
    statsButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #404040;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #505050;"
        "}"
        );

    leftLayout->addWidget(usersButton);
    leftLayout->addWidget(statsButton);
    leftLayout->addStretch();

    // Кнопка выхода
    QPushButton *exitButton = new QPushButton("Выход");
    exitButton->setFixedHeight(40);
    exitButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #ea4335;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #d33426;"
        "}"
        );
    leftLayout->addWidget(exitButton);

    mainLayout->addWidget(leftPanel);

    // Правая часть (контент)
    QFrame *contentFrame = new QFrame();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentFrame);
    contentLayout->setContentsMargins(20, 20, 20, 20);

    // Заголовок
    QLabel *titleLabel = new QLabel("Статистика тестирования");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 20px;");
    contentLayout->addWidget(titleLabel);

    // Таблица статистики
    QTableWidget *statsTable = new QTableWidget();
    statsTable->setColumnCount(3);
    statsTable->setHorizontalHeaderLabels(QStringList() << "ФИО студента" << "Тест" << "Результат");
    statsTable->setRowCount(10); // Установим фиксированное количество строк

    // Настройка таблицы
    statsTable->setStyleSheet(
        "QTableWidget {"
        "   border: 1px solid #e0e0e0;"
        "   font-size: 12pt;"
        "   selection-background-color: #e0e0e0;"
        "}"
        "QHeaderView::section {"
        "   background-color: black;"
        "   color: white;"
        "   padding: 8px;"
        "   border: none;"
        "}"
        );

    statsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    statsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    statsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    statsTable->verticalHeader()->setVisible(false);
    statsTable->horizontalHeader()->setStretchLastSection(true);

    // Установка ширины колонок
    statsTable->setColumnWidth(0, 350);
    statsTable->setColumnWidth(1, 300);

    // Заполнение таблицы данными
    QStringList names = {"Иванов Иван Иванович", "Петров Петр Петрович", "Сидорова Анна Михайловна",
                         "Кузнецов Дмитрий Сергеевич", "Павлова Ольга Игоревна"};
    QStringList tests = {"Математика", "Физика", "Информатика", "История", "Английский язык"};

    for (int i = 0; i < 5; ++i) {
        QTableWidgetItem *nameItem = new QTableWidgetItem(names[i]);
        nameItem->setTextAlignment(Qt::AlignCenter);
        statsTable->setItem(i, 0, nameItem);

        QTableWidgetItem *testItem = new QTableWidgetItem(tests[i]);
        testItem->setTextAlignment(Qt::AlignCenter);
        statsTable->setItem(i, 1, testItem);

        QTableWidgetItem *resultItem = new QTableWidgetItem(QString::number(70 + i*5) + "%");
        resultItem->setTextAlignment(Qt::AlignCenter);
        statsTable->setItem(i, 2, resultItem);
    }

    contentLayout->addWidget(statsTable);

    mainLayout->addWidget(contentFrame, 1);

    // Подключение сигналов
    connect(exitButton, &QPushButton::clicked, this, &AdminStatWindow::close);
    connect(usersButton, &QPushButton::clicked, this, &AdminStatWindow::showUsersWindow);
}

AdminStatWindow::~AdminStatWindow()
{
    delete ui;
}

void AdminStatWindow::showUsersWindow()
{
    hide();
    AdminWindow *adminWindow = new AdminWindow();
    adminWindow->show();
}
