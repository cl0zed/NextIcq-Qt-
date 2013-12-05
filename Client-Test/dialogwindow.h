#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

#include <QDialog>
#include "mainwidget.h"
#include "myclient.h"
#include <QString>
#include <QTabWidget>
#include <QTextEdit>
class MyClient;
class mainWidget;

class dialogWindow : public QDialog
{
    Q_OBJECT
private:
    QTabWidget* mainTab;

    MyClient* client;
    QTextEdit* currentMessage;
    QWidget* createTab();
    QString clientName;
//protected:
    //void closeEvent(QCloseEvent *);
public:
    explicit dialogWindow(mainWidget* parent = 0,
                          MyClient* socket = 0,
                          const QString& str = QString() );
    QTextEdit* allMessages;
public slots:
    void slotSend();
};

#endif // DIALOGWINDOW_H
