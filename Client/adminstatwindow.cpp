#include "adminstatwindow.h"
#include "adminwindow.h"
#include "ui_adminstatwindow.h"
#include <QApplication>
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
    resize(800, 500);
    this->setFixedSize(800, 500);
    setWindowTitle("Администратор - Статистика");
    setStyleSheet("background-color: white; color: black;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Шапка
    QFrame *headerFrame = new QFrame();
    headerFrame->setStyleSheet("background-color: black; color: white; padding: 0px;");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);

    QLabel *roleLabel = new QLabel("Администратор");
    roleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    headerLayout->addWidget(roleLabel);

    headerLayout->addStretch();

    mainLayout->addWidget(headerFrame);

    QHBoxLayout *contentLayout = new QHBoxLayout();

    // Левая колонка (меню)
    QFrame *leftColumn = new QFrame();
    leftColumn->setStyleSheet("padding: 10px;");
    QVBoxLayout *leftLayout = new QVBoxLayout(leftColumn);

    QPushButton *usersButton = new QPushButton("Пользователи");
    usersButton->setStyleSheet("QPushButton { background-color: black; color: white; padding: 10px; border: none; }"
                               "QPushButton:hover { background-color: #333; }");

    QPushButton *statsButton = new QPushButton("Статистика");
    statsButton->setStyleSheet("QPushButton { background-color: gray; color: white; padding: 10px; border: none; }"
                               "QPushButton:hover { background-color: #333; }");

    leftLayout->addWidget(usersButton);
    leftLayout->addWidget(statsButton);
    leftLayout->addStretch();

    QPushButton *exitButton = new QPushButton("ВЫХОД");
    exitButton->setStyleSheet("QPushButton { background: #ea4335; color: white; min-width: 40px; min-height: 20px; font-size: 11.5pt; border: none; padding-left: 10px; padding-top: 4px; padding-right: 10px; padding-bottom: 4px; max-height: 10px; margin-right: 0px}"
                              "QPushButton:hover { background: #d33426; }");
    exitButton->setIconSize(QSize(32, 32));
    leftLayout->addWidget(exitButton);

    mainLayout->addWidget(leftColumn, 1);

    // Правая колонка (контент - статистика)
    // Таблица статистики
    QTableWidget *statsTable = new QTableWidget();
    statsTable->setColumnCount(3);
    statsTable->setHorizontalHeaderLabels(QStringList() << "ФИО студента" << "Тест" << "Результат");

    // Настройка внешнего вида таблицы
    statsTable->setStyleSheet("QTableWidget { border: 1px solid #e0e0e0; font-size: 12pt; margin-left: 5px; }"
                              "QHeaderView::section { background-color: #f4f4f4; padding: 5px; }");
    statsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    statsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    statsTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // Размеры колонок
    statsTable->setColumnWidth(0, 300);
    statsTable->setColumnWidth(1, 200);

    //заполнение
    QTableWidgetItem *item1 = new QTableWidgetItem("Иванов Иван Иванович");
    item1->setTextAlignment(Qt::AlignCenter);
    statsTable->setItem(0, 0, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem("Математика");
    item2->setTextAlignment(Qt::AlignCenter);
    statsTable->setItem(0, 1, item2);

    QTableWidgetItem *item3 = new QTableWidgetItem("85%");
    item3->setTextAlignment(Qt::AlignCenter);
    statsTable->setItem(0, 2, item3);

    QTableWidgetItem *item22 = new QTableWidgetItem("Павлова Ольга Игоревна");
    item22->setTextAlignment(Qt::AlignCenter);
    statsTable->setItem(7, 0, item22);

    QTableWidgetItem *item23 = new QTableWidgetItem("Информатика");
    item23->setTextAlignment(Qt::AlignCenter);
    statsTable->setItem(7, 1, item23);

    QTableWidgetItem *item24 = new QTableWidgetItem("95%");
    item24->setTextAlignment(Qt::AlignCenter);
    statsTable->setItem(7, 2, item24);


    contentLayout->addWidget(statsTable, 3);

    mainLayout->addWidget(statsTable, 1);

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
    AdminWindow *statWindow = new AdminWindow();
    statWindow->show();
}
