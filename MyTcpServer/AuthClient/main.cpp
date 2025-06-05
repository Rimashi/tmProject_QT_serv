#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Создаем главное окно
    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
