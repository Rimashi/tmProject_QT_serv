#ifndef CLIENTAPI_H
#define CLIENTAPI_H

#include <QWidget>

namespace Ui {
class ClientAPI;
}

class ClientAPI : public QWidget
{
    Q_OBJECT

public:
    explicit ClientAPI(QWidget *parent = nullptr);
    ~ClientAPI();

private:
    Ui::ClientAPI *ui;
};

#endif // CLIENTAPI_H
