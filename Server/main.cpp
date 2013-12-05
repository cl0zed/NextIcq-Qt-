#include "server.h"
#include <QApplication>
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MyServer server(2324);

    server.show();
    server.setVisible(false);

    return app.exec();
}
