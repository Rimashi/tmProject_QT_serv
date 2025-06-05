#ifndef TEST2DIALOG_H
#define TEST2DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QBuffer>
#include "client_api.h"

class Test2Dialog : public QDialog {
    Q_OBJECT
public:
    Test2Dialog(const QString& task, const QString& login, ClientAPI* api, QWidget* parent = nullptr);

private slots:
    void onSelectImage();
    void onSubmit();
    void onHideTask();

private:
    QString studentLogin;
    QString currentTask;
    ClientAPI* api;
    QImage selectedImage;
    QLabel* imageLabel;
    QLabel* statusLabel;
    QPushButton* selectBtn;
    QPushButton* submitBtn;

    QLabel* taskLabel;
    QPushButton* hideBtn;
    bool isTaskHidden = false;
};

#endif // TEST2DIALOG_H
