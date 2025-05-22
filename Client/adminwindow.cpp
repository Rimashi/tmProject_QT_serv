#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "adminstatwindow.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QListWidget>
#include <QFrame>
#include <QLineEdit>
#include <QSpacerItem>

AdminWindow::AdminWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    resize(800, 500);
    this->setFixedSize(800, 500);
    setWindowTitle("Администратор");
    setStyleSheet("background-color: white; color: black;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Шапка
    QFrame *headerFrame = new QFrame();
    headerFrame->setStyleSheet("background-color: black; color: white; padding: 0px;");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);

    QLabel *roleLabel = new QLabel("Администратор");
    roleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    headerLayout->addWidget(roleLabel);

    headerLayout->addStretch();

    mainLayout->addWidget(headerFrame);

    QHBoxLayout *contentLayout = new QHBoxLayout();

    // Левая колонка (меню)
    QFrame *leftColumn = new QFrame();
    leftColumn->setStyleSheet("padding: 10px;");
    QVBoxLayout *leftLayout = new QVBoxLayout(leftColumn);

    QPushButton *usersButton = new QPushButton("Пользователи");
    usersButton->setStyleSheet("QPushButton { background-color: gray; color: white; padding: 10px; border: none; }"
                               "QPushButton:hover { background-color: #333; }");

    QPushButton *statsButton = new QPushButton("Статистика");
    statsButton->setStyleSheet("QPushButton { background-color: black; color: white; padding: 10px; border: none; }"
                               "QPushButton:hover { background-color: #333; }");

    leftLayout->addWidget(usersButton);
    leftLayout->addWidget(statsButton);
    leftLayout->addStretch();

    QPushButton *exitButton = new QPushButton("ВЫХОД");
    exitButton->setStyleSheet("QPushButton { background: #ea4335; color: white; min-width: 40px; min-height: 20px; font-size: 11.5pt; border: none; padding-left: 10px; padding-top: 4px; padding-right: 10px; padding-bottom: 4px; max-height: 10px; margin-right: 0px}"
                              "QPushButton:hover { background: #d33426; }");
    exitButton->setIconSize(QSize(32, 32));
    leftLayout->addWidget(exitButton);

    contentLayout->addWidget(leftColumn, 1);

    // Правая колонка (контент)
    QFrame *rightColumn = new QFrame();
    rightColumn->setStyleSheet("padding: 0px;");
    QVBoxLayout *rightLayout = new QVBoxLayout(rightColumn);

    // Панель поиска
    QFrame *searchPanel = new QFrame();
    QHBoxLayout *searchLayout = new QHBoxLayout(searchPanel);

    searchLineEdit = new QLineEdit();
    searchLineEdit->setPlaceholderText("Поиск пользователей...");
    searchLineEdit->setStyleSheet("padding: 5px; border: 1px solid black;");

    QPushButton *searchButton = new QPushButton("Найти");
    searchButton->setStyleSheet("QPushButton { background-color: black; color: white; padding: 5px; border: none; }"
                                "QPushButton:hover { background-color: #333; }");

    searchLayout->addWidget(searchLineEdit, 1);
    searchLayout->addWidget(searchButton);

    rightLayout->addWidget(searchPanel);

    QLabel *usersLabel = new QLabel("Список пользователей:");
    rightLayout->addWidget(usersLabel);

    // Область с пользователями
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);

    scrollContent = new QWidget();
    scrollLayout = new QVBoxLayout(scrollContent);
/*
    for (int i = 1; i <= 15; ++i) {
        QFrame *userFrame = new QFrame();
        userFrame->setStyleSheet("border: 1px solid black; padding: 0px; margin-bottom: 0px;");
        QHBoxLayout *userLayout = new QHBoxLayout(userFrame);

        QLabel *userLabel = new QLabel(QString("Пользователь %1").arg(i));
        QPushButton *viewButton = new QPushButton("Просмотр");
        viewButton->setStyleSheet("QPushButton { background-color: black; color: white; padding: 5px; border: none; }"
                                  "QPushButton:hover { background-color: #333; }");

        userLayout->addWidget(userLabel, 1);
        userLayout->addWidget(viewButton);

        scrollLayout->addWidget(userFrame);
    }
*/

    scrollLayout->addStretch();
    scrollArea->setWidget(scrollContent);
    rightLayout->addWidget(scrollArea);

    contentLayout->addWidget(rightColumn, 3);
    mainLayout->addLayout(contentLayout, 1);

    // Подключаем сигналы
    connect(exitButton, &QPushButton::clicked, this, &AdminWindow::close);
    connect(searchButton, &QPushButton::clicked, this, &AdminWindow::searchUsers);
    connect(statsButton, &QPushButton::clicked, this, &AdminWindow::goView);
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::searchUsers()
{
    QString searchText = searchLineEdit->text().toLower();

    // Скрываем/показываем пользователей в зависимости от поискового запроса
    for (int i = 0; i < scrollLayout->count(); ++i) {
        QWidget *widget = scrollLayout->itemAt(i)->widget();
        if (widget) {
            QFrame *userFrame = qobject_cast<QFrame*>(widget);
            if (userFrame) {
                QLabel *userLabel = userFrame->findChild<QLabel*>();
                if (userLabel) {
                    bool match = userLabel->text().toLower().contains(searchText);
                    userFrame->setVisible(match || searchText.isEmpty());
                }
            }
        }
    }
}

void AdminWindow::goView(){
    hide();
    AdminStatWindow *goViewWindow = new AdminStatWindow();
    goViewWindow->show();
}

