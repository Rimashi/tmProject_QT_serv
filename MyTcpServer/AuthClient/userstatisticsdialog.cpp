#include "userstatisticsdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QJsonArray>
#include <QDateTime>
#include <QTableWidget>
#include <QJsonObject>

UserStatisticsDialog::UserStatisticsDialog(const QJsonObject& data, QWidget* parent)
    : QDialog(parent)
{
    setupUI(data);
}

void UserStatisticsDialog::setupUI(const QJsonObject& data) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Заголовок с ФИО пользователя
    QString name = data.value("surname").toString() + " " + data.value("name").toString();
    QLabel* nameLabel = new QLabel("Статистика пользователя: <b>" + name + "</b>", this);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setStyleSheet("font-size: 14pt; margin: 10px;");
    layout->addWidget(nameLabel);

    QJsonArray stats = data.value("statistics").toArray();

    if (stats.isEmpty()) {
        QLabel* emptyLabel = new QLabel("Данные о прохождении тестов отсутствуют", this);
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("font-size: 12pt; color: gray; margin-top: 50px;");
        layout->addWidget(emptyLabel);
    } else {
        QTableWidget* table = new QTableWidget(this);
        table->setColumnCount(3);
        table->setHorizontalHeaderLabels(QStringList() << "№ теста" << "Дата" << "Результат");
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->verticalHeader()->setVisible(false);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionMode(QAbstractItemView::NoSelection);
        table->setRowCount(stats.size());

        for (int i = 0; i < stats.size(); ++i) {
            QJsonObject item = stats[i].toObject();

            // Форматирование даты
            QString isoDate = item["date"].toString();
            QDateTime dateTime = QDateTime::fromString(isoDate, Qt::ISODate);
            QString formattedDate = dateTime.toString("dd.MM.yyyy HH:mm");

            table->setItem(i, 0, new QTableWidgetItem(item["test_number"].toString()));
            table->setItem(i, 1, new QTableWidgetItem(formattedDate));
            table->setItem(i, 2, new QTableWidgetItem(item["score"].toString()));
        }

        layout->addWidget(table);
    }

    setLayout(layout);
}
