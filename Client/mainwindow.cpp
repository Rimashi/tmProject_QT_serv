#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "statisticwindow.h"
#include "testwindow1.h"
#include "testwindow2.h"
#include "testwindow3.h"
#include "testwindow4.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Тестирующая система");
    resize(800, 300);
    this->setFixedSize(800, 300);


    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);


    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(20);


    // Шапка
    QFrame *headerFrame = new QFrame();
    headerFrame->setStyleSheet("background: #f4f4f4; border-bottom: 4px solid #e0e0e0; padding: 0px; max-height: 80px");


    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);

    // пользователь
    QWidget *userWidget = new QWidget();
    QHBoxLayout *userLayout = new QHBoxLayout(userWidget);
    userWidget->setStyleSheet("border: 0px solid #f4f4f4; align: left; margin-left: 0px");

    headerLayout->addWidget(userWidget);
    mainLayout->addWidget(headerFrame);

    headerLayout->addStretch();

    // Заголовок
    QLabel *titleLabel = new QLabel("ТЕСТОВАЯ СИСТЕМА");
    titleLabel->setStyleSheet("font-weight: bold; border: 0px solid #f4f4f4; font-size: 20pt; align: center");
    headerLayout->addWidget(titleLabel);

    // status
    QPushButton *statisticButton = new QPushButton("СТАТИСТИКА");
    statisticButton->setStyleSheet("QPushButton { background: #34a853; color: white; border-radius: 5px; min-width: 40px; min-height: 20px; font-size: 11.5pt; border: none; padding-left: 10px; padding-top: 4px; padding-right: 10px; padding-bottom: 4px; max-height: 10px; align: right}"
                                 "QPushButton:hover { background: #2d9248; }");
    statisticButton->setIconSize(QSize(32, 32));
    headerLayout->addStretch();
    headerLayout->addWidget(statisticButton);

    // выход
    QPushButton *exitButton = new QPushButton("ВЫХОД");
    exitButton->setStyleSheet("QPushButton { background: #ea4335; color: white; border-radius: 5px; min-width: 40px; min-height: 20px; font-size: 11.5pt; border: none; padding-left: 10px; padding-top: 4px; padding-right: 10px; padding-bottom: 4px; max-height: 10px; margin-right: 0px}"
                              "QPushButton:hover { background: #d33426; }");
    exitButton->setIconSize(QSize(32, 32));
    headerLayout->addWidget(exitButton);

    mainLayout->addLayout(headerLayout);

    // сетка для тестов
    QGridLayout *gridLayout = new QGridLayout();

    // тест 1
    QPushButton *test1Button = new QPushButton("ТЕСТ 1");
    test1Button->setStyleSheet("QPushButton { background: #000000; color: white; border-radius: 10px; min-width: 150px; min-height: 150px; font-weight: bold; font-size: 14pt; border: none; }"
                               "QPushButton:hover { background: #696969; }");
    test1Button->setIconSize(QSize(48, 48));
    gridLayout->addWidget(test1Button, 0, 0);

    // тест 2
    QPushButton *test2Button = new QPushButton("ТЕСТ 2");
    test2Button->setStyleSheet(test1Button->styleSheet());
    test2Button->setIconSize(QSize(48, 48));
    gridLayout->addWidget(test2Button, 0, 1);
/*
    // тест 3
    QPushButton *test3Button = new QPushButton("ТЕСТ 3");
    test3Button->setStyleSheet(test1Button->styleSheet());
    test3Button->setIconSize(QSize(48, 48));
    gridLayout->addWidget(test3Button, 1, 0);

    // тест 4
    QPushButton *test4Button = new QPushButton("ТЕСТ 4");
    test4Button->setStyleSheet(test1Button->styleSheet());
    test4Button->setIconSize(QSize(48, 48));
    gridLayout->addWidget(test4Button, 1, 1);
*/
    mainLayout->addLayout(gridLayout);

    connect(exitButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(statisticButton, &QPushButton::clicked, this, &MainWindow::goStat);
    connect(test1Button, &QPushButton::clicked, this, &MainWindow::goTest1);
    connect(test2Button, &QPushButton::clicked, this, &MainWindow::goTest2);
//    connect(test3Button, &QPushButton::clicked, this, &MainWindow::goTest3);
//    connect(test4Button, &QPushButton::clicked, this, &MainWindow::goTest4);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::goStat(){
    hide();
    StatisticWindow *statWindow = new StatisticWindow();
    statWindow->show();
}

void MainWindow::goTest1(){
    hide();
    TestWindow1 *test1Window = new TestWindow1();
    test1Window->show();
}

void MainWindow::goTest2(){
    hide();
    TestWindow2 *test2Window = new TestWindow2();
    test2Window->show();
}

void MainWindow::goTest3(){
    hide();
    TestWindow3 *test3Window = new TestWindow3();
    test3Window->show();
}

void MainWindow::goTest4(){
    hide();
    TestWindow4 *test4Window = new TestWindow4();
    test4Window->show();
}

