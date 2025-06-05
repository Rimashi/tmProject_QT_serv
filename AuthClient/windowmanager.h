#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>
#include <QMainWindow>
#include <QJsonObject>

class ClientAPI;
class QWidget;

class WindowManager : public QObject {
    Q_OBJECT
public:
    explicit WindowManager(ClientAPI* api, QMainWindow* parent = nullptr);

    void showAuthScreen();
    void showMainScreen(const QJsonObject& userData);
    void showMainScreenForAdmin();

signals:
    void exitRequested();

private:
    QMainWindow* mainWindow;
    ClientAPI* clientAPI;
    QWidget* currentScreen = nullptr;
};

#endif // WINDOWMANAGER_H
