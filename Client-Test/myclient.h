#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>
#include <QString>
#include <QStringList>
#include "mainwidget.h"

class mainWidget;
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

    static const quint8 messageGetMessage = 1;
    static const quint8 messageGetContactList = 2;
public:

    QStringList contacts;
    explicit MyClient(const QString& , int, mainWidget *parent = 0);
    void setMyName(const QString&);
    void setSenderName(const QString&);
    void setMessage(const QString&);

    QString takeMyName();
    QString takeMessage();

    void NewRegiter();

    void sendContactListRequest();
    void waitMessage();
    void waitSendMessage();
    void send();
    void sendMessageToDelete();

private slots:
    void slotReadReady();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();

    
};

#endif // MYCLIENT_H
