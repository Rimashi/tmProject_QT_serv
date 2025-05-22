#include "testwindow1.h"
#include "ui_testwindow1.h"
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

TestWindow1::TestWindow1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestWindow1)
{
    ui->setupUi(this);
    setWindowTitle("Тестовое задание 1");
    resize(800, 500);
    this->setFixedSize(800, 500);

    QWidget *Test1Widget = new QWidget(this);


    QVBoxLayout *mainTest1Layout = new QVBoxLayout(Test1Widget);
    mainTest1Layout->setSpacing(20);


    // шапка
    QFrame *headerTest1Frame = new QFrame();
    headerTest1Frame->setStyleSheet("background: #f4f4f4; border-bottom: 4px solid #e0e0e0; margin-left: 5px; min-width: 320px");


    QHBoxLayout *headerTest1Layout = new QHBoxLayout(headerTest1Frame);

    // пользователь
    QWidget *userTest1Widget = new QWidget();
    QHBoxLayout *userTest1Layout = new QHBoxLayout(userTest1Widget);
    userTest1Widget->setStyleSheet("border: 0px solid #f4f4f4; align: left; margin-left: 0px");


    headerTest1Layout->addWidget(userTest1Widget);
    mainTest1Layout->addWidget(headerTest1Frame);

    headerTest1Layout->addStretch();

    // заголовок
    QLabel *titleTest1Label = new QLabel("Test 1");
    titleTest1Label->setStyleSheet("font-weight: bold; border: 0px solid #f4f4f4; font-size: 20pt; align: center");
    headerTest1Layout->addWidget(titleTest1Label);

    // ВЫХОД
    QPushButton *exitTest1Button = new QPushButton("ВЫХОД");
    exitTest1Button->setStyleSheet("QPushButton { background: #DC143C; color: white; border-radius: 5px; min-width: 40px; min-height: 20px; font-size: 11.5pt; border: none; padding-left: 10px; padding-top: 4px; padding-right: 10px; padding-bottom: 4px; max-height: 10px; margin-right: 0px}"
                              "QPushButton:hover { background: #d33426; }");
    exitTest1Button->setIconSize(QSize(32, 32));
    headerTest1Layout->addWidget(exitTest1Button);

    mainTest1Layout->addLayout(headerTest1Layout);


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

    mainTest1Layout->addWidget(scrollArea, 1);

    QHBoxLayout *bottomTest1Layout = new QHBoxLayout();
    bottomTest1Layout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QPushButton *closeTest1Button = new QPushButton("СКРЫТЬ ЗАДАЧУ");
    closeTest1Button->setStyleSheet("QPushButton { background: #34a853; color: white; border-radius: 5px; min-width: 40px; min-height: 20px; font-size: 11.5pt; border: none; padding-left: 10px; padding-top: 4px; padding-right: 10px; padding-bottom: 4px; max-height: 10px; align: right}"
                                   "QPushButton:hover { background: #2d9248; }");
    closeTest1Button->setIconSize(QSize(32, 32));

    bottomTest1Layout->addWidget(closeTest1Button);
    mainTest1Layout->addLayout(bottomTest1Layout);

    connect(exitTest1Button, &QPushButton::clicked, this, &TestWindow1::close);
    connect(closeTest1Button, &QPushButton::clicked, this, &TestWindow1::goMainTest1);
}

TestWindow1::~TestWindow1()
{
    delete ui;
}

void TestWindow1::goMainTest1(){
    MainWindow *mainWindowTest1 = new MainWindow();
    mainWindowTest1->show();
    this->close();
}

/* Метод для установки текста теста
void TestWindow1::setTestContent(const QString &testText, int testNumber)
{
    // Обновляем номер теста в заголовке
    titleTest1Label->setText(QString("Test %1").arg(testNumber));

    // Устанавливаем текст теста
    testContent->setPlainText(testText);
}
*/
