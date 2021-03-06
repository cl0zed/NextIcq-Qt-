#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QList>

struct MyQPoint
{
    QPoint point;
    bool repaintWithNext;
    MyQPoint(QPoint thisPoint) {
        point = thisPoint;
        repaintWithNext=true;
    }
    MyQPoint()
    {
        repaintWithNext = false;
    }
};

struct HostSocket
{
    QString userName;
    QTcpSocket* socket;
};

class MyServer : public QWidget
{
    Q_OBJECT
private:
    QTcpServer* server;
    quint16 nextBlockSize;
    QList<HostSocket> sockets;
    QList<MyQPoint> points;

    void sendMessage(QTcpSocket* , const QString& , const QString &name);
    void sendContactBase(QTcpSocket*);
    void sendPoint(QTcpSocket*, QList<MyQPoint>&, bool );

public:
    explicit MyServer(int, QWidget *parent = 0);
    int Port;
    static const quint8 cmdMessage = 1;
    static const quint8 cmdUserName = 2;
    static const quint8 cmdGetContactList = 3;
    static const quint8 cmdGetDeleteMessage = 4;
    static const quint8 cmdGetPoints = 5;

public slots:
    void slotNewConnection();
    void slotClientRead();
    //void slotDeleteNow();
};

#endif // SERVER_H
