#include "testwindow4.h"
#include "ui_testwindow4.h"
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

TestWindow4::TestWindow4(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestWindow4)
{
    ui->setupUi(this);
    setWindowTitle("Тестовое задание 4");
    resize(800, 500);
    this->setFixedSize(800, 500);

    QWidget *Test4Widget = new QWidget(this);


    QVBoxLayout *mainTest4Layout = new QVBoxLayout(Test4Widget);
    mainTest4Layout->setSpacing(20);


    // шапка
    QFrame *headerTest4Frame = new QFrame();
    headerTest4Frame->setStyleSheet("background: #f4f4f4; border-bottom: 4px solid #e0e0e0; margin-left: 5px; min-width: 320px");


    QHBoxLayout *headerTest4Layout = new QHBoxLayout(headerTest4Frame);

    // пользователь
    QWidget *userTest4Widget = new QWidget();
    QHBoxLayout *userTest4Layout = new QHBoxLayout(userTest4Widget);
    userTest4Widget->setStyleSheet("border: 0px solid #f4f4f4; align: left; margin-left: 0px");

    // Ник
    QLabel *userNameTest4Label = new QLabel("User name");
    userNameTest4Label->setStyleSheet("font-size: 14pt; border-bottom: 0px solid #f4f4f4");
    userTest4Layout->addWidget(userNameTest4Label);

    headerTest4Layout->addWidget(userTest4Widget);
    mainTest4Layout->addWidget(headerTest4Frame);

    headerTest4Layout->addStretch();

    // заголовок
    QLabel *titleTest4Label = new QLabel("Test 4");
    titleTest4Label->setStyleSheet("font-weight: bold; border: 0px solid #f4f4f4; font-size: 20pt; align: center");
    headerTest4Layout->addWidget(titleTest4Label);

    // назад
    QPushButton *exitTest4Button = new QPushButton("ВЫХОД");
    exitTest4Button->setStyleSheet("QPushButton { background: #DC143C; color: white; border-radius: 5px; min-width: 40px; min-height: 20px; font-size: 11.5pt; border: none; padding-left: 10px; padding-top: 4px; padding-right: 10px; padding-bottom: 4px; max-height: 10px; margin-right: 0px}"
                              "QPushButton:hover { background: #d33426; }");
    exitTest4Button->setIconSize(QSize(32, 32));
    headerTest4Layout->addWidget(exitTest4Button);

    mainTest4Layout->addLayout(headerTest4Layout);

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

    mainTest4Layout->addWidget(scrollArea, 1);

    QHBoxLayout *bottomTest4Layout = new QHBoxLayout();
    bottomTest4Layout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QPushButton *closeTest4Button = new QPushButton("СКРЫТЬ ЗАДАЧУ");
    closeTest4Button->setStyleSheet("QPushButton { background: #34a853; color: white; border-radius: 5px; min-width: 40px; min-height: 20px; font-size: 11.5pt; border: none; padding-left: 10px; padding-top: 4px; padding-right: 10px; padding-bottom: 4px; max-height: 10px; align: right}"
                                    "QPushButton:hover { background: #2d9248; }");
    closeTest4Button->setIconSize(QSize(32, 32));

    bottomTest4Layout->addWidget(closeTest4Button);
    mainTest4Layout->addLayout(bottomTest4Layout);

    connect(exitTest4Button, &QPushButton::clicked, this, &TestWindow4::close);
    connect(closeTest4Button, &QPushButton::clicked, this, &TestWindow4::goMainTest4);



}

TestWindow4::~TestWindow4()
{
    delete ui;
}

void TestWindow4::goMainTest4(){
    MainWindow *mainWindowTest4 = new MainWindow();
    mainWindowTest4->show();
    this->close();
}
