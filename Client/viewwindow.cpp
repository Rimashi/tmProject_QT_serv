#include "viewwindow.h"
#include "ui_viewwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QScrollBar>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

ViewWindow::ViewWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ViewWindow)
{
    ui->setupUi(this);
    // Настройка основного окна
    setWindowTitle("Статистика пользователя");
    setFixedSize(400, 300);
    setStyleSheet("background-color: white; color: black;");

    // Главный layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Шапка (header)
    QFrame *headerFrame = new QFrame();
    headerFrame->setStyleSheet("background-color: black; padding: 10px;");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);

    QLabel *titleLabel = new QLabel("Статистика пользователя");
    titleLabel->setStyleSheet("color: white; font-weight: bold; font-size: 14px;");
    headerLayout->addWidget(titleLabel);

    headerLayout->addStretch();

    QPushButton *backButton = new QPushButton("Назад");
    backButton->setStyleSheet("QPushButton { background-color: #333; color: white; border: none; padding: 5px 10px; }"
                              "QPushButton:hover { background-color: #555; }");
    headerLayout->addWidget(backButton);

    mainLayout->addWidget(headerFrame);

    // Область с таблицей
    QFrame *contentFrame = new QFrame();
    contentFrame->setStyleSheet("background-color: white; border: none;");
    QVBoxLayout *contentLayout = new QVBoxLayout(contentFrame);

    // Таблица для статистики
    tableWidget = new QTableWidget();
    tableWidget->setStyleSheet("QTableWidget { border: 1px solid #ddd; }"
                               "QHeaderView::section { background-color: #f0f0f0; padding: 5px; border: 1px solid #ddd; }"
                               "QTableWidget::item { border: 1px solid #ddd; padding: 5px; }"
                               "QScrollBar:vertical { width: 12px; }");

    // Настройка таблицы
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "№ теста" << "Результат");
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setShowGrid(true);

    // Добавляем таблицу в Scroll Area
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(tableWidget);
    scrollArea->setFrameShape(QFrame::NoFrame);
    contentLayout->addWidget(scrollArea);

    mainLayout->addWidget(contentFrame, 1);

    // Подключаем кнопку назад
    connect(backButton, &QPushButton::clicked, this, &ViewWindow::close);
}

ViewWindow::~ViewWindow()
{
    delete ui;
}

void ViewWindow::setTestResults(const QVector<TestResult> &results)
{
    tableWidget->setRowCount(results.size());

    for (int i = 0; i < results.size(); ++i) {
        const TestResult &result = results[i];

        QTableWidgetItem *testNumberItem = new QTableWidgetItem(QString::number(result.testNumber));
        testNumberItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *resultItem = new QTableWidgetItem(QString::number(result.score) + "%");
        resultItem->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 0, testNumberItem);
        tableWidget->setItem(i, 1, resultItem);
    }

    // Оптимизируем ширину столбцов
    tableWidget->resizeColumnsToContents();
}
