#include "test2dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QImageReader>
#include <QDebug>

Test2Dialog::Test2Dialog(const QString& task, const QString& login, ClientAPI* api, QWidget* parent)
    : QDialog(parent), studentLogin(login), currentTask(task), api(api)
{
    setWindowTitle("Тест 2 - Стеганография");
    setMinimumSize(600, 400);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Заголовок
    QLabel* titleLabel = new QLabel("Тестовое задание 2: Стеганография", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18pt; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    // Описание
    QLabel* descLabel = new QLabel("Встройте следующую битовую последовательность в изображение:", this);
    mainLayout->addWidget(descLabel);

    // Задание
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

    // Кнопка выбора изображения
    selectBtn = new QPushButton("Выбрать изображение", this);
    mainLayout->addWidget(selectBtn);

    // Статус выбора
    statusLabel = new QLabel("Изображение не выбрано", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);

    // Превью изображения
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setMinimumHeight(200);
    imageLabel->setStyleSheet("border: 1px solid #ccc;");
    mainLayout->addWidget(imageLabel);

    // Кнопка отправки
    submitBtn = new QPushButton("Отправить", this);
    submitBtn->setEnabled(false);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(submitBtn);
    mainLayout->addLayout(btnLayout);

    connect(selectBtn, &QPushButton::clicked, this, &Test2Dialog::onSelectImage);
    connect(submitBtn, &QPushButton::clicked, this, &Test2Dialog::onSubmit);
}

void Test2Dialog::onSelectImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите изображение", "",
                                                    "PNG Images (*.png)");
    if (fileName.isEmpty()) return;

    QFileInfo fileInfo(fileName);
    if (fileInfo.size() > 2 * 1024 * 1024) { // 2MB
        QMessageBox::warning(this, "Ошибка", "Изображение слишком большое (максимум 2MB)");
        return;
    }

    QImageReader reader(fileName);
    reader.setAutoTransform(true);

    if (!reader.canRead()) {
        QMessageBox::warning(this, "Ошибка", "Невозможно прочитать изображение");
        return;
    }

    QSize imageSize = reader.size();
    if (imageSize.width() > 4096 || imageSize.height() > 4096) {
        QMessageBox::warning(this, "Ошибка", "Слишком большие размеры изображения (макс. 4096x4096)");
        return;
    }

    selectedImage = reader.read();
    if (selectedImage.isNull()) {
        QMessageBox::warning(this, "Ошибка", "Ошибка загрузки изображения");
        return;
    }

    // Показываем превью
    QPixmap pixmap = QPixmap::fromImage(selectedImage);
    if (pixmap.width() > 500 || pixmap.height() > 300) {
        pixmap = pixmap.scaled(500, 300, Qt::KeepAspectRatio);
    }
    imageLabel->setPixmap(pixmap);
    statusLabel->setText("Выбрано: " + fileName);
    submitBtn->setEnabled(true);
}


void Test2Dialog::onSubmit() {
    if (selectedImage.isNull()) {
        QMessageBox::warning(this, "Ошибка", "Изображение не выбрано");
        return;
    }

    // Конвертируем в PNG
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);

    // Сохраняем в формате PNG
    if (!selectedImage.save(&buffer, "PNG")) {
        QMessageBox::warning(this, "Ошибка", "Не удалось преобразовать изображение");
        return;
    }

    // Отправляем на сервер
    api->sendTest2(studentLogin, currentTask, imageData);
    QMessageBox::information(this, "Успех", "Изображение отправлено на проверку");
    accept();
}

void Test2Dialog::onHideTask() {
    isTaskHidden = !isTaskHidden;
    taskLabel->setVisible(!isTaskHidden);
    hideBtn->setText(isTaskHidden ? "Показать задачу" : "Скрыть задачу");
}
