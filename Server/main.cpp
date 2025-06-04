#include "server_api.h"
#include <QCoreApplication>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    Server_api server;
    return app.exec();
}
