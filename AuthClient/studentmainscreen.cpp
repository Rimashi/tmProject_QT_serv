// studentmainscreen.cpp
#include "studentmainscreen.h"
#include "statisticsdialog.h"
#include "test1dialog.h"
#include "studentmainscreen.h"
#include "statisticsdialog.h"
#include "test1dialog.h"
#include "test2dialog.h"
#include <QMessageBox>
#include <QJsonArray>
#include <QThread>
#include <QHBoxLayout>
#include <QFrame>

StudentMainScreen::StudentMainScreen(const QString& login, ClientAPI* clientAPI, QWidget* parent)
    : QWidget(parent), studentLogin(login), api(clientAPI)
{
    // Установка основного стиля
    this->setStyleSheet(
        "QWidget { background-color: white; }"
        "QFrame#header { "
        "    background-color: white; "  // Бежевый фон заголовка
        "    border-bottom: 2px solid black; "
        "    padding: 10px; "
        "}"
        "QLabel#title { "
        "    font-size: 24px; "
        "    font-weight: bold; "
        "    color: black; "
        "}"
        "QFrame#divider { "
        "    background-color: black; "  // Черный цвет разделителя
        "    margin: 0 10px; "
        "}"
        );

    // Устанавливаем фиксированный размер окна (шире)
    this->setFixedSize(800, 600);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Заголовок
    QFrame* header = new QFrame(this);
    header->setObjectName("header");
    QHBoxLayout* headerLayout = new QHBoxLayout(header);
    QLabel* title = new QLabel("ТЕСТОВАЯ СИСТЕМА", header);
    title->setObjectName("title");
    headerLayout->addWidget(title, 0, Qt::AlignCenter);
    mainLayout->addWidget(header);

    // Основное содержимое
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(20, 15, 20, 15);
    contentLayout->setSpacing(20);

    // Левый столбец
    QVBoxLayout* leftColumn = new QVBoxLayout();
    leftColumn->setSpacing(10);

    btnTests = new QPushButton("ТЕСТЫ", this);
    btnStatistics = new QPushButton("СТАТИСТИКА", this);

    btnExit = new QPushButton("ВЫХОД", this);

    // Стиль для кнопок левого столбца
    QString leftButtonStyle =
        "QPushButton { "
        "    background-color: black; "
        "    color: white; "
        "    border-radius: 10px; "
        "    padding: 10px; "
        "    font-size: 16px; "
        "    font-weight: bold; "
        "    min-height: 40px; "
        "}"
        "QPushButton:hover { "
        "    background-color: #333333; "
        "}";

    btnTests->setStyleSheet(leftButtonStyle);
    btnStatistics->setStyleSheet(leftButtonStyle);
    btnExit->setStyleSheet(leftButtonStyle);

    // Делаем кнопку ТЕСТЫ неактивной
    btnTests->setEnabled(false);

    leftColumn->addWidget(btnTests);
    leftColumn->addWidget(btnStatistics);
    leftColumn->addStretch();
    leftColumn->addWidget(btnExit);

    // Разделитель столбцов (черный)
    QFrame* divider = new QFrame(this);
    divider->setObjectName("divider");
    divider->setFixedWidth(2);
    divider->setStyleSheet("background-color: black;");

    // Правый столбец
    QVBoxLayout* rightColumn = new QVBoxLayout();
    rightColumn->setSpacing(15);

    btnTest1 = new QPushButton("ТЕСТ 1", this);
    btnTest2 = new QPushButton("ТЕСТ 2", this);

    // Увеличиваем размер кнопок
    QSizePolicy policy = btnTest1->sizePolicy();
    policy.setVerticalStretch(25);
    btnTest1->setSizePolicy(policy);
    btnTest2->setSizePolicy(policy);

    // Стиль для кнопок правого столбца
    QString rightButtonStyle =
        "QPushButton { "
        "    background-color: black; "
        "    color: white; "
        "    border-radius: 10px; "
        "    padding: 30px; "
        "    font-size: 20px; "
        "    font-weight: bold; "
        "    min-height: 80px; "
        "}"
        "QPushButton:hover { "
        "    background-color: #333333; "  // Серый при наведении
        "}";

    btnTest1->setStyleSheet(rightButtonStyle);
    btnTest2->setStyleSheet(rightButtonStyle);

    rightColumn->addWidget(btnTest1);
    rightColumn->addWidget(btnTest2);
    rightColumn->addStretch();

    contentLayout->addLayout(leftColumn, 1);
    contentLayout->addWidget(divider);
    contentLayout->addLayout(rightColumn, 1);
    mainLayout->addLayout(contentLayout, 1);

    // Подключение сигналов
    connect(btnStatistics, &QPushButton::clicked, this, &StudentMainScreen::onStatisticsClicked);
    connect(api, &ClientAPI::statisticsReceived, this, &StudentMainScreen::handleStatisticsReceived);
    connect(btnTest1, &QPushButton::clicked, this, &StudentMainScreen::onTest1Clicked);
    connect(btnTest2, &QPushButton::clicked, this, &StudentMainScreen::onTest2Clicked);
    connect(btnExit, &QPushButton::clicked, this, &StudentMainScreen::onExitClicked);
    connect(api, &ClientAPI::task1Received, this, &StudentMainScreen::handleTask1Received);
    connect(api, &ClientAPI::task2Received, this, &StudentMainScreen::handleTask2Received);
}

void StudentMainScreen::onStatisticsClicked() {
    api->getStatisticsStudent(studentLogin); // Отправляем запрос на статистику
}

void StudentMainScreen::onTest1Clicked() {
    QMessageBox::information(this, "Загрузка", "Получение задания...");

    // Отправляем запрос на сервер
    api->getTest1(studentLogin);
}

void StudentMainScreen::onTest2Clicked() {
    api->getTest2(studentLogin);
}

void StudentMainScreen::onExitClicked() {
    emit exitRequested();
}

void StudentMainScreen::handleStatisticsReceived(const QJsonObject& data) {
    qDebug() << "Received statistics data:" << data;

    // Проверяем структуру данных
    if (data.contains("surname") && data.contains("name") && data.contains("statistics")) {
        // Проверяем, что statistics - массив
        if (data["statistics"].isArray()) {
            // Если массив пустой, покажем сообщение
            if (data["statistics"].toArray().isEmpty()) {
                QMessageBox::information(this, "Статистика", "Данные о прохождении тестов отсутствуют");
            } else {
                StatisticsDialog* dialog = new StatisticsDialog(data, this);
                dialog->exec();
                dialog->deleteLater();
            }
        } else {
            qWarning() << "Statistics is not an array";
            QMessageBox::warning(this, "Ошибка", "Некорректный формат данных статистики");
        }
    } else {
        qWarning() << "Missing required fields in statistics data";
        QMessageBox::warning(this, "Ошибка", "Отсутствуют обязательные поля в данных статистики");
    }
}

void StudentMainScreen::handleTask1Received(const QString& task) {
    qDebug() << "=== ENTER handleTask1Received ===";
    qDebug() << "Thread ID:" << QThread::currentThreadId();
    qDebug() << "Task content:" << task;

    // Создаем и показываем диалог с заданием
    qDebug() << "Creating Test1Dialog...";
    Test1Dialog* dialog = new Test1Dialog(task, studentLogin, api, this);
    qDebug() << "Test1Dialog created. Showing...";
    dialog->exec();
    qDebug() << "Test1Dialog closed.";
    dialog->deleteLater();
    qDebug() << "=== EXIT handleTask1Received ===";
}

void StudentMainScreen::handleTask2Received(const QString& task) {
    Test2Dialog* dialog = new Test2Dialog(task, studentLogin, api, this);
    dialog->exec();
    dialog->deleteLater();
}
