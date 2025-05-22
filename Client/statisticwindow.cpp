#include "statisticwindow.h"
#include "ui_statisticwindow.h"
#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QHeaderView>
#include <QSizePolicy>

StatisticWindow::StatisticWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StatisticWindow)
{
    ui->setupUi(this);

    setWindowTitle("Статистика");
    resize(800, 500);
    this->setFixedSize(800, 500);

    QWidget *statisticWidget = new QWidget(this);


    QVBoxLayout *mainStatisticLayout = new QVBoxLayout(statisticWidget);
    mainStatisticLayout->setSpacing(20);
    mainStatisticLayout->setContentsMargins(0, 0, 0, 0);


    // шапка
    QFrame *headerStatisticFrame = new QFrame();
    headerStatisticFrame->setStyleSheet("background: #f4f4f4; border-bottom: 4px solid #e0e0e0; margin-left: 5px; min-width: 330px");
    headerStatisticFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //QWidget *headerStat = new QWidget();
    QHBoxLayout *headerStatisticLayout = new QHBoxLayout(headerStatisticFrame);

    // пользователь
    QWidget *userStatisticWidget = new QWidget();
    QHBoxLayout *userStatisticLayout = new QHBoxLayout(userStatisticWidget);
    userStatisticWidget->setStyleSheet("border: 0px solid #f4f4f4; align: left; margin: 0px");
    userStatisticWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    headerStatisticLayout->addWidget(userStatisticWidget);
    mainStatisticLayout->addWidget(headerStatisticFrame);



    // заголовок
    QLabel *titleStatisticLabel = new QLabel("Статистика");
    titleStatisticLabel->setStyleSheet("font-weight: bold; border: 0px solid #f4f4f4; font-size: 20pt");
    titleStatisticLabel->setAlignment(Qt::AlignLeft);
    headerStatisticLayout->addWidget(titleStatisticLabel);
    headerStatisticLayout->addStretch();

    // Кнопка выхода
    QPushButton *exitStatisticButton = new QPushButton("ВЫХОД");
    exitStatisticButton->setStyleSheet("QPushButton { background: #DC143C; color: white; border-radius: 5px; min-width: 40px; min-height: 20px; font-size: 11.5pt; border: none; padding-left: 10px; padding-top: 4px; padding-right: 10px; padding-bottom: 4px; max-height: 10px}"
                              "QPushButton:hover { background: #d33426; }");
    exitStatisticButton->setIconSize(QSize(32, 32));
    headerStatisticLayout->addWidget(exitStatisticButton);

    mainStatisticLayout->addLayout(headerStatisticLayout);

    //Вывод статистики на экран

    QTableWidget *statisticTable = new QTableWidget();
    statisticTable->setColumnCount(3);
    statisticTable->setHorizontalHeaderLabels(QStringList() << "ФИО студента" << "№ теста" << "Результат");

    // Внешний вид таблицы
    statisticTable->setStyleSheet("QTableWidget { border: 1px solid #e0e0e0; font-size: 12pt; margin-left: 5px}"
                                  "QHeaderView::section { background-color: #f4f4f4; padding: 5px;}");
    statisticTable->horizontalHeader()->setStretchLastSection(true);
    statisticTable->verticalHeader()->setVisible(false);
    statisticTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    statisticTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    statisticTable->setSelectionMode(QAbstractItemView::SingleSelection);
/*
    statisticTable->setRowCount(3);
    statisticTable->setItem(0, 0, new QTableWidgetItem("Иванов И.И."));
    statisticTable->setItem(0, 1, new QTableWidgetItem("1"));
    statisticTable->setItem(0, 2, new QTableWidgetItem("85%"));

    statisticTable->setItem(1, 0, new QTableWidgetItem("Петров П.П."));
    statisticTable->setItem(1, 1, new QTableWidgetItem("2"));
    statisticTable->setItem(1, 2, new QTableWidgetItem("92%"));

    statisticTable->setItem(2, 0, new QTableWidgetItem("Сидоров С.С."));
    statisticTable->setItem(2, 1, new QTableWidgetItem("1"));
    statisticTable->setItem(2, 2, new QTableWidgetItem("78%"));
*/
    // Размеры колонок
    statisticTable->setColumnWidth(0, 300);
    statisticTable->setColumnWidth(1, 100);

    mainStatisticLayout->addWidget(statisticTable, 1);

    QHBoxLayout *bottomStatisticLayout = new QHBoxLayout();
    bottomStatisticLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // статистика
    QPushButton *statisticBackButton = new QPushButton("Убрать статистику");
    statisticBackButton->setStyleSheet("QPushButton { background: #808000; color: white; border-radius: 5px; min-width: 40px; min-height: 20px; font-size: 11.5pt; border: none; padding-left: 10px; padding-top: 4px; padding-right: 10px; margin-right: 10px; padding-bottom: 4px; min-height: 40px; align: right}"
                                   "QPushButton:hover { background: #2d9248; }");
    statisticBackButton->setIconSize(QSize(32, 32));
    bottomStatisticLayout->addStretch();
    bottomStatisticLayout->addWidget(statisticBackButton);
    mainStatisticLayout->addLayout(bottomStatisticLayout);

    statisticWidget->setLayout(mainStatisticLayout);


    connect(exitStatisticButton, &QPushButton::clicked, this, &StatisticWindow::close);
    connect(statisticBackButton, &QPushButton::clicked, this, &StatisticWindow::goMainStat);

}

StatisticWindow::~StatisticWindow()
{
    delete ui;
}

void StatisticWindow::goMainStat(){
    MainWindow *mainWindowStat = new MainWindow();
    mainWindowStat->show();
    this->close();
}

/*
void StatisticWindow::setStatisticsData(const QList<StudentTestResult> &results)
{
    statisticTable->setRowCount(results.size());

    for (int i = 0; i < results.size(); ++i) {
        const StudentTestResult &result = results[i];
        statisticTable->setItem(i, 0, new QTableWidgetItem(result.studentName));
        statisticTable->setItem(i, 1, new QTableWidgetItem(QString::number(result.testNumber)));
        statisticTable->setItem(i, 2, new QTableWidgetItem(QString::number(result.score) + "%"));
    }
}
*/


