#include "test1dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFont>
#include <QDebug>

Test1Dialog::Test1Dialog(const QString& task, const QString& login, ClientAPI* clientAPI, QWidget* parent)
    : QDialog(parent), studentLogin(login), currentTask(task), api(clientAPI)
{
    qDebug() << "=== Test1Dialog CONSTRUCTOR START ===";

    setWindowTitle("Тест 1");
    setMinimumSize(600, 400);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);

    // Заголовок теста
    QLabel* titleLabel = new QLabel("Тестовое задание 1", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18pt; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    // Метод решения
    QLabel* methodLabel = new QLabel("Решите уравнение методом хорд", this);
    methodLabel->setAlignment(Qt::AlignCenter);
    methodLabel->setStyleSheet("font-size: 14pt; font-style: italic;");
    mainLayout->addWidget(methodLabel);

    // Область с заданием
    QFrame* taskFrame = new QFrame(this);
    taskFrame->setFrameShape(QFrame::Box);
    taskFrame->setMinimumHeight(100); // Гарантируем видимость области
    taskFrame->setStyleSheet(
        "background-color: #f8f8f8;"
        "padding: 15px;"
        "border: 1px solid #cccccc;"
        "border-radius: 5px;"
        );

    qDebug() << "try to set task";

    QVBoxLayout* taskLayout = new QVBoxLayout(taskFrame);

    QLabel* taskHeader = new QLabel("Задание:", taskFrame);
    taskHeader->setStyleSheet("font-weight: bold; font-size: 12pt;");

    taskLabel = new QLabel(task, taskFrame);
    taskLabel->setTextFormat(Qt::PlainText); // Важное исправление!
    taskLabel->setWordWrap(true);
    taskLabel->setStyleSheet(
        "font-size: 12pt;"
        "margin-top: 10px;"
        "color: #333333;" // Явно задаем цвет текста
        );

    taskLayout->addWidget(taskHeader);
    taskLayout->addWidget(taskLabel);

    mainLayout->addWidget(taskFrame);

    // +++ ДОБАВЛЯЕМ ПОЛЕ ДЛЯ ВВОДА ОТВЕТА +++
    // Поле для ответа
    QHBoxLayout* answerLayout = new QHBoxLayout();
    answerLayout->setSpacing(10);

    QLabel* answerLabel = new QLabel("Ваш ответ:", this);
    answerLabel->setStyleSheet("font-size: 12pt;");

    answerInput = new QLineEdit(this);
    answerInput->setMinimumHeight(30);
    answerInput->setStyleSheet("font-size: 12pt;");

    answerLayout->addWidget(answerLabel);
    answerLayout->addWidget(answerInput);
    mainLayout->addLayout(answerLayout);
    // +++++++++++++++++++++++++++++++++++++++

    // Кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    // Сохраняем кнопку в член класса, чтобы можно было к ней обращаться
    hideBtn = new QPushButton("Скрыть задачу", this);
    hideBtn->setStyleSheet("font-size: 11pt; padding: 8px;");

    QPushButton* submitBtn = new QPushButton("Отправить", this);
    submitBtn->setStyleSheet("font-size: 11pt; padding: 8px; background-color: #4CAF50; color: white;");

    QPushButton* exitBtn = new QPushButton("Выход", this);
    exitBtn->setStyleSheet("font-size: 11pt; padding: 8px; background-color: #f44336; color: white;");

    buttonLayout->addWidget(hideBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(exitBtn);
    buttonLayout->addWidget(submitBtn);

    mainLayout->addLayout(buttonLayout);

    Q_ASSERT(taskLabel != nullptr);
    Q_ASSERT(answerInput != nullptr);
    Q_ASSERT(hideBtn != nullptr);
    Q_ASSERT(api != nullptr);

    qDebug() << "UI elements initialized";
    qDebug() << "Task text:" << task;
    qDebug() << "=== Test1Dialog CONSTRUCTOR END ===";

    // Соединения
    connect(submitBtn, &QPushButton::clicked, this, &Test1Dialog::onSubmit);
    connect(hideBtn, &QPushButton::clicked, this, &Test1Dialog::onHideTask);
    connect(exitBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void Test1Dialog::onSubmit() {
    QString answer = answerInput->text().trimmed();

    if (answer.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите ваш ответ");
        return;
    }

    // Проверка, что ответ - число
    bool isNumber;
    answer.toDouble(&isNumber);
    if (!isNumber) {
        QMessageBox::warning(this, "Ошибка", "Ответ должен быть числом");
        return;
    }

    // Отправляем ответ на сервер
    api->sendTest1(studentLogin, currentTask, answer);

    // Закрываем диалог
    accept();
}

void Test1Dialog::onHideTask() {
    isTaskHidden = !isTaskHidden;
    taskLabel->setVisible(!isTaskHidden);
    hideBtn->setText(isTaskHidden ? "Показать задачу" : "Скрыть задачу");
}
