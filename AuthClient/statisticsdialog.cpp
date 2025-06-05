#include "statisticsdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QJsonArray>
#include <QDateTime>
#include <QLineEdit>
#include <QPushButton>

StatisticsDialog::StatisticsDialog(const QJsonObject& data, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Статистика студента");
    setMinimumSize(600, 400); // Увеличил минимальный размер окна
    setupUI(data);
}

void StatisticsDialog::setupUI(const QJsonObject& data) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // Заголовок "Статистика студента"
    QLabel* titleLabel = new QLabel("СТАТИСТИКА СТУДЕНТА", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 16pt;"
        "font-weight: bold;"
        "color: #333333;"
        "margin-bottom: 15px;"
        );
    mainLayout->addWidget(titleLabel);

    QJsonArray stats = data["statistics"].toArray();

    if (stats.isEmpty()) {
        QLabel* emptyLabel = new QLabel("Данные о прохождении тестов отсутствуют", this);
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet(
            "font-size: 12pt;"
            "color: gray;"
            "margin-top: 50px;"
            );
        mainLayout->addWidget(emptyLabel);
    } else {
        // Информация о студенте
        QString name = data["surname"].toString() + " " + data["name"].toString();
        QLabel* nameLabel = new QLabel("Студент: <b>" + name + "</b>", this);
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setStyleSheet(
            "font-size: 12pt;"
            "color: #555555;"
            "margin-bottom: 15px;"
            );
        mainLayout->addWidget(nameLabel);

        // Таблица с данными
        table = new QTableWidget(this);
        table->setColumnCount(3);
        table->setHorizontalHeaderLabels(QStringList() << "№ теста" << "Дата" << "Результат");
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->verticalHeader()->setVisible(false);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionMode(QAbstractItemView::NoSelection);
        table->setStyleSheet(
            "QTableWidget {"
            "   border: 1px solid #cccccc;"
            "   border-radius: 5px;"
            "}"
            "QHeaderView::section {"
            "   background-color: #f0f0f0;"
            "   padding: 5px;"
            "   border: none;"
            "   font-weight: bold;"
            "}"
            "QTableWidget::item {"
            "   padding: 5px;"
            "}"
            );

        table->setRowCount(stats.size());

        for (int i = 0; i < stats.size(); ++i) {
            QJsonObject item = stats[i].toObject();
            QString isoDate = item["date"].toString();
            QDateTime dateTime = QDateTime::fromString(isoDate, Qt::ISODate);
            QString formattedDate = dateTime.toString("dd.MM.yyyy HH:mm");

            table->setItem(i, 0, new QTableWidgetItem(item["test_number"].toString()));
            table->setItem(i, 1, new QTableWidgetItem(formattedDate));
            table->setItem(i, 2, new QTableWidgetItem(item["score"].toString()));
        }

        mainLayout->addWidget(table);

        // Поисковая строка
        QHBoxLayout* searchLayout = new QHBoxLayout();
        searchLayout->setContentsMargins(0, 10, 0, 0);

        QLabel* searchLabel = new QLabel("Поиск по дате:", this);
        searchLabel->setStyleSheet("font-size: 10pt;");

        QLineEdit* searchLineEdit = new QLineEdit(this);
        searchLineEdit->setPlaceholderText("Введите дату...");
        searchLineEdit->setStyleSheet(
            "QLineEdit {"
            "   border: 1px solid #cccccc;"
            "   border-radius: 3px;"
            "   padding: 5px;"
            "}"
            );

        QPushButton* searchButton = new QPushButton("Найти", this);
        searchButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #4CAF50;"
            "   color: white;"
            "   border: none;"
            "   border-radius: 3px;"
            "   padding: 5px 15px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #45a049;"
            "}"
            );

        searchLayout->addWidget(searchLabel);
        searchLayout->addWidget(searchLineEdit);
        searchLayout->addWidget(searchButton);

        mainLayout->addLayout(searchLayout);

        // Подключение поиска
        connect(searchLineEdit, &QLineEdit::textChanged, this, [this](const QString& text) {
            for (int i = 0; i < table->rowCount(); ++i) {
                bool match = table->item(i, 1)->text().contains(text, Qt::CaseInsensitive);
                table->setRowHidden(i, !match && !text.isEmpty());
            }
        });
    }

    setLayout(mainLayout);
}
