#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "myclient.h"
#include <QSettings>
#include <QListWidget>
#include <QMenu>
#include <QLineEdit>
#include "dialogwindow.h"

class dialogWindow;
class MyClient;
class mainWidget : public QWidget
{
    Q_OBJECT
private:
    MyClient* client;
    QSettings settings;
    QMenu* contextMenu;
    QLineEdit* setName;


    void createMenu();
    void writeSettings();
    void readSettings();

protected:
    void closeEvent(QCloseEvent *);
public:
    dialogWindow* dialog ;
    QListWidget* contacts;
    explicit mainWidget(QWidget *parent = 0);

public slots:
    void slotAddFriend();
    void slotDialogStart();
    void slotDialogStart(QListWidgetItem*);
    void slotMenuRequested(QPoint);
    void slotDeleteFromContactList();
    void slotSetName();
    void slotStartPaint();
};

#endif // MAINWIDGET_H
