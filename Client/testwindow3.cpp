#include "testwindow3.h"
#include "ui_testwindow3.h"
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

TestWindow3::TestWindow3(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestWindow3)
{
    ui->setupUi(this);
    setWindowTitle("Тестовое задание 3");
    resize(800, 500);
    this->setFixedSize(800, 500);

    QWidget *Test3Widget = new QWidget(this);


    QVBoxLayout *mainTest3Layout = new QVBoxLayout(Test3Widget);
    mainTest3Layout->setSpacing(20);


    // шапка
    QFrame *headerTest3Frame = new QFrame();
    headerTest3Frame->setStyleSheet("background: #f4f4f4; border-bottom: 4px solid #e0e0e0; margin-left: 5px; min-width: 320px");


    QHBoxLayout *headerTest3Layout = new QHBoxLayout(headerTest3Frame);

    // пользователь
    QWidget *userTest3Widget = new QWidget();
    QHBoxLayout *userTest3Layout = new QHBoxLayout(userTest3Widget);
    userTest3Widget->setStyleSheet("border: 0px solid #f4f4f4; align: left; margin-left: 0px");


    // Ник
    QLabel *userNameTest3Label = new QLabel("User name");
    userNameTest3Label->setStyleSheet("font-size: 14pt; border-bottom: 0px solid #f4f4f4");
    userTest3Layout->addWidget(userNameTest3Label);

    headerTest3Layout->addWidget(userTest3Widget);
    mainTest3Layout->addWidget(headerTest3Frame);

    headerTest3Layout->addStretch();

    // заголовок
    QLabel *titleTest3Label = new QLabel("Test 3");
    titleTest3Label->setStyleSheet("font-weight: bold; border: 0px solid #f4f4f4; font-size: 20pt; align: center");
    headerTest3Layout->addWidget(titleTest3Label);

    // назад
    QPushButton *exitTest3Button = new QPushButton("ВЫХОД");
    exitTest3Button->setStyleSheet("QPushButton { background: #DC143C; color: white; border-radius: 5px; min-width: 40px; min-height: 20px; font-size: 11.5pt; border: none; padding-left: 10px; padding-top: 4px; padding-right: 10px; padding-bottom: 4px; max-height: 10px; margin-right: 0px}"
                              "QPushButton:hover { background: #d33426; }");
    exitTest3Button->setIconSize(QSize(32, 32));
    headerTest3Layout->addWidget(exitTest3Button);

    mainTest3Layout->addLayout(headerTest3Layout);

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

    mainTest3Layout->addWidget(scrollArea, 1);

    QHBoxLayout *bottomTest3Layout = new QHBoxLayout();
    bottomTest3Layout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QPushButton *closeTest3Button = new QPushButton("СКРЫТЬ ЗАДАЧУ");
    closeTest3Button->setStyleSheet("QPushButton { background: #34a853; color: white; border-radius: 5px; min-width: 40px; min-height: 20px; font-size: 11.5pt; border: none; padding-left: 10px; padding-top: 4px; padding-right: 10px; padding-bottom: 4px; max-height: 10px; align: right}"
                                    "QPushButton:hover { background: #2d9248; }");
    closeTest3Button->setIconSize(QSize(32, 32));

    bottomTest3Layout->addWidget(closeTest3Button);
    mainTest3Layout->addLayout(bottomTest3Layout);

    connect(exitTest3Button, &QPushButton::clicked, this, &TestWindow3::close);
    connect(closeTest3Button, &QPushButton::clicked, this, &TestWindow3::goMainTest3);



}

TestWindow3::~TestWindow3()
{
    delete ui;
}

void TestWindow3::goMainTest3(){
    MainWindow *mainWindowTest3 = new MainWindow();
    mainWindowTest3->show();
    this->close();
}
