#include <QApplication>
#include "mainwidget.h"
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    /*MyClient client("127.0.0.1", 2324);
    client.show();*/

    QCoreApplication::setOrganizationName("Test2");
    QCoreApplication::setApplicationName("Myicq");

    mainWidget icq;
    icq.show();


    return app.exec();
}
