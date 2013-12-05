#include "server.h"
#include <QDataStream>
#include <QByteArray>
#include <QTcpSocket>
#include <QTime>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
void MyServer::sendMessage(QTcpSocket *socket, const QString &str, const QString& name)
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    QString toSend = "take << [" + QTime::currentTime().toString() + "] " + name + ": " + str;
    out << quint16(0) << quint8(1) <<  toSend;

    out.device()->seek(0);
    out << quint16(array.size() - sizeof(quint16));

    socket->write(array);
    qDebug() << "Message send ";
}

void MyServer::sendContactBase(QTcpSocket * socket)
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);



    out << quint16(0) << quint8(2) << sockets.count();

    QList<HostSocket>::const_iterator pos;

    for ( pos=sockets.constBegin(); pos!=sockets.constEnd(); ++pos )
    {
        out << pos ->userName;
        qDebug() << pos -> userName;
    }

    out.device()->seek(0);
    out << quint16(array.size() - sizeof(quint16));
    socket->write(array);

    qDebug() << "Contacts send";
}

MyServer::MyServer(int nPort, QWidget *parent) :
    QWidget(parent), nextBlockSize(0), Port(nPort)
{
    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any, nPort))
    {
        QMessageBox::critical(0 , "Server Error", "Unable to start server: " + server->errorString());
        server->close();
        return;
    }
    qDebug() << "Server started ";
    connect(server, SIGNAL(newConnection()), SLOT(slotNewConnection()));
}



void MyServer::slotNewConnection()
{
    QTcpSocket* socket = server->nextPendingConnection();

    qDebug() << "New connection with" << socket->peerAddress();


    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotClientRead()));

    //sendMessage(socket, "Successful connected");
}

void MyServer::slotClientRead()
{
    QTcpSocket* client = (QTcpSocket*) sender();

    QDataStream in(client);

    in.setVersion(QDataStream::Qt_4_5);
    for (;;)
    {
        if (!nextBlockSize)
        {
            if (client->bytesAvailable() < sizeof(quint16))
            {
                break;
            }
            in >> nextBlockSize;
        }
        if (client->bytesAvailable() < nextBlockSize)
        {
            break;
        }

        // output << quint16 << command << QHostAddress << QString
        // output << size    << command << IP           << msg

        quint8 command;
        in >> command;
        qDebug() << "Take a comand " << command;
        switch (command)
        {
            case MyServer::cmdMessage :
            {
                QString userName;
                in >> userName;

                QString str;
                in >> str;

                qDebug() << userName << " " << str;

                QList<HostSocket>::const_iterator pos;
                for ( pos=sockets.constBegin(); pos!=sockets.constEnd(); ++pos )
                {
                    if ( pos->userName == userName )
                    {
                        qDebug() << "Send to " << pos->userName;
                        sendMessage(pos->socket, str, pos->userName);
                    }
                }
                break;

            }

            case MyServer::cmdUserName :
            {
                bool isFound = false;
                QString userName;
                in >> userName;

                HostSocket newSocket;
                newSocket.userName = userName;
                newSocket.socket = client;

                QList<HostSocket>::const_iterator pos;
                for ( pos=sockets.constBegin(); pos!=sockets.constEnd(); ++pos )
                {
                    if ( pos->userName == newSocket.userName )
                        isFound = true;
                }
                if (!isFound)
                {
                    sockets.push_back(newSocket);
                    qDebug() << "new User " << newSocket.userName;
                }
                break;

            }
            case MyServer::cmdGetContactList :
            {
                QString str;
                in >> str;
                qDebug() << str;
                QList<HostSocket>::const_iterator pos;
                for ( pos=sockets.constBegin(); pos!=sockets.constEnd(); ++pos )
                {
                    if ( pos->userName == str )
                    {
                        sendContactBase(pos->socket);
                    }
                }
                break;
            }
            case MyServer::cmdGetDeleteMessage :
            {

                QList<HostSocket>::iterator pos;
                for (pos=sockets.begin(); pos!=sockets.end(); ++pos )
                {
                    if (pos -> socket == client)
                    {
                        qDebug() << sockets.count() << "Delete " << pos -> userName;
                        sockets.erase(pos);
                        qDebug() << sockets.count();
                    }
                }
                break;
            }
        default:break;
        }

    }

}

/*void MyServer::slotDeleteNow()
{
    QTcpSocket* client = (QTcpSocket*) sender();

    deleteLater();
}*/

