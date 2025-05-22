#include "clientapi.h"
#include "ui_clientapi.h"

ClientAPI::ClientAPI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientAPI)
{
    ui->setupUi(this);
}

ClientAPI::~ClientAPI()
{
    delete ui;
}
