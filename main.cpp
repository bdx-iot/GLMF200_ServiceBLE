#include <QCoreApplication>

#include "CServerBLE.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CServerBLE *serverBLE = new CServerBLE();

    serverBLE->initServer();

    return a.exec();
}
