#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>
#include <QString>
#include <QStringList>
#include "mainwidget.h"
#include "draw.h"

class mainWidget;
struct MyQPoint;

class MyClient : public QWidget
{
    Q_OBJECT
private:
    QTcpSocket* socket;
    QString myName;
    QString clientName;
    QString message;
    QString messageFromServer;
    quint16 nextBlockSize;
    mainWidget* parentMainWidget;
    QList<MyQPoint> PointList;

    static const quint8 messageGetMessage = 1;
    static const quint8 messageGetContactList = 2;
    static const quint8 messageGetPointList = 3;
public:

    QStringList contacts;
    explicit MyClient(const QString& , int, mainWidget *parent = 0);
    void setMyName(const QString&);
    void setSenderName(const QString&);
    void setMessage(const QString&);

    QString takeMyName();
    QString takeMessage();
    QString takeClientName();

    void NewRegiter();

    void sendContactListRequest();
    void waitMessage();
    void waitSendMessage();
    void send();
    void sendMessageToDelete();
    void sendPaintedPoints(QList<MyQPoint>& );
private slots:
    void slotReadReady();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();

    
};

#endif // MYCLIENT_H
