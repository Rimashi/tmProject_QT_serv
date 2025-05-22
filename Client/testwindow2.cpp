#include "testwindow2.h"
#include "ui_testwindow2.h"
#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QScrollArea>

TestWindow2::TestWindow2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestWindow2)
{
    ui->setupUi(this);
    setWindowTitle("Тестовое задание 2");
    resize(800, 500);
    this->setFixedSize(800, 500);

    QWidget *Test2Widget = new QWidget(this);


    QVBoxLayout *mainTest2Layout = new QVBoxLayout(Test2Widget);
    mainTest2Layout->setSpacing(20);


    // шапка
    QFrame *headerTest2Frame = new QFrame();
    headerTest2Frame->setStyleSheet("background: #f4f4f4; border-bottom: 4px solid #e0e0e0; margin-left: 5px; min-width: 320px");


    QHBoxLayout *headerTest2Layout = new QHBoxLayout(headerTest2Frame);

    // пользователь
    QWidget *userTest2Widget = new QWidget();
    QHBoxLayout *userTest2Layout = new QHBoxLayout(userTest2Widget);
    userTest2Widget->setStyleSheet("border: 0px solid #f4f4f4; align: left; margin-left: 0px");

    headerTest2Layout->addWidget(userTest2Widget);
    mainTest2Layout->addWidget(headerTest2Frame);

    headerTest2Layout->addStretch();

    // заголовок
    QLabel *titleTest2Label = new QLabel("Test 2");
    titleTest2Label->setStyleSheet("font-weight: bold; border: 0px solid #f4f4f4; font-size: 20pt; align: center");
    headerTest2Layout->addWidget(titleTest2Label);

    // назад
    QPushButton *exitTest2Button = new QPushButton("ВЫХОД");
    exitTest2Button->setStyleSheet("QPushButton { background: #DC143C; color: white; border-radius: 5px; min-width: 40px; min-height: 20px; font-size: 11.5pt; border: none; padding-left: 10px; padding-top: 4px; padding-right: 10px; padding-bottom: 4px; max-height: 10px; margin-right: 0px}"
                                   "QPushButton:hover { background: #d33426; }");
    exitTest2Button->setIconSize(QSize(32, 32));
    headerTest2Layout->addWidget(exitTest2Button);

    mainTest2Layout->addLayout(headerTest2Layout);

    // Виджет для отображения текста теста
    QTextEdit *testContent = new QTextEdit();
    testContent->setReadOnly(true);  // Только для чтения
    testContent->setStyleSheet("QTextEdit { background: white; border: 1px solid #e0e0e0; font-size: 12pt; padding: 10px; }");

    /* Пример текста теста
    QString testText = "Текст тестового задания, полученный с сервера.\n\n"
                       "1. Первый вопрос теста...\n"
                       "2. Второй вопрос теста...\n"
                       "3. Третий вопрос теста...\n\n"
                       "Выберите правильные варианты ответов.";
    testContent->setPlainText(testText);
    */

    // область прокрутки
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(testContent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    mainTest2Layout->addWidget(scrollArea, 1);

    QHBoxLayout *bottomTest2Layout = new QHBoxLayout();
    bottomTest2Layout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QPushButton *closeTest2Button = new QPushButton("СКРЫТЬ ЗАДАЧУ");
    closeTest2Button->setStyleSheet("QPushButton { background: #34a853; color: white; border-radius: 5px; min-width: 40px; min-height: 20px; font-size: 11.5pt; border: none; padding-left: 10px; padding-top: 4px; padding-right: 10px; padding-bottom: 4px; max-height: 10px; align: right}"
                                    "QPushButton:hover { background: #2d9248; }");
    closeTest2Button->setIconSize(QSize(32, 32));

    bottomTest2Layout->addWidget(closeTest2Button);
    mainTest2Layout->addLayout(bottomTest2Layout);

    connect(exitTest2Button, &QPushButton::clicked, this, &TestWindow2::close);
    connect(closeTest2Button, &QPushButton::clicked, this, &TestWindow2::goMainTest2);

}

TestWindow2::~TestWindow2()
{
    delete ui;
}

void TestWindow2::goMainTest2(){
    MainWindow *mainWindowTest2 = new MainWindow();
    mainWindowTest2->show();
    this->close();
}
