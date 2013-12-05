#include <QDataStream>
#include <QTime>
#include <QByteArray>
#include <QMessageBox>
#include <QDebug>
#include "myclient.h"

void MyClient::NewRegiter()
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    out << quint16(0) << quint8(2) << myName;

    out.device()->seek(0);
    socket->write(array);

    qDebug() << "UserName send " << myName;
}

void MyClient::sendContactListRequest()
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    out << quint16(0) << quint8(3) << myName;

    out.device()->seek(0);
    socket->write(array);

    qDebug() << "ContactList request send ";
}

void MyClient::waitMessage()
{
    socket->waitForReadyRead();
}

void MyClient::waitSendMessage()
{
    socket->waitForBytesWritten(3000);
}

void MyClient::send()
{
    slotSendToServer();
}

void MyClient::sendMessageToDelete()
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    out << quint16(0) << quint8(4);

    out.device()->seek(0);
    socket->write(array);
}


MyClient::MyClient(const QString &str, int Port, mainWidget *parent) :
    QWidget(parent), nextBlockSize(0)
{
    parentMainWidget = parent;
    socket = new QTcpSocket(this);
    socket->connectToHost(str, Port);


    connect(socket, SIGNAL(readyRead()), SLOT(slotReadReady()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));

}

void MyClient::setMyName(const QString &str)
{
    myName = str;
    qDebug() << myName;
}

void MyClient::setSenderName(const QString &str)
{
    clientName = str;
}

void MyClient::setMessage(const QString &str)
{
    message = str;
}

QString MyClient::takeMyName()
{
    return myName;
}

QString MyClient::takeMessage()
{
    return messageFromServer;
}

void MyClient::slotReadReady()
{
    QDataStream stream(socket);
    stream.setVersion(QDataStream::Qt_4_5);
    for (;;)
    {
        if (!nextBlockSize)
        {
            if (socket->bytesAvailable() < sizeof(quint16))
            {
                break;
            }
            stream >> nextBlockSize;
        }
        if (socket->bytesAvailable() < nextBlockSize)
        {
            break;
        }
        quint8 cmd;

        stream >> cmd;
        qDebug() << "Send command " << cmd;

        switch (cmd)
        {
            case MyClient::messageGetMessage :
            {
                stream >> messageFromServer;
                qDebug() << messageFromServer;
                if (parentMainWidget != 0)
                {
                    parentMainWidget->dialog->allMessages->append(messageFromServer);
                }
                break;
            }
            case MyClient::messageGetContactList :
            {
                int numberofContacts;
                stream >> numberofContacts;
                qDebug() << numberofContacts ;
                for (int i = 0 ; i < numberofContacts; ++i)
                {
                    QString str;
                    stream >> str;
                    qDebug() << str;
                    contacts << str;
                }
                qDebug() << "Contacts read";
                break;
            }
        }



        nextBlockSize = 0;
    }
}

void MyClient::slotError(QAbstractSocket::SocketError)
{
    QMessageBox::critical(0, "Error ",   QString(socket->errorString()) );
}

void MyClient::slotSendToServer()
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    out << quint16(0) << quint8(1) << clientName <<  message;

    out.device()->seek(0);
    socket->write(array);

}

void MyClient::slotConnected()
{
    //textEdit->append("Recived Connected() signal");
}
