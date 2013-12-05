#ifndef ADDFRIENDWINDOW_H
#define ADDFRIENDWINDOW_H

#include <QDialog>
#include "mainwidget.h"
#include <QListWidget>

class addFriendWindow : public QDialog
{
    Q_OBJECT
private:
    QListWidget* allContacts;
    mainWidget* parentMainWidget;
    void addCurrentItem(const QString& );
public:
    explicit addFriendWindow(mainWidget *parent = 0, MyClient* socket = 0);
    
signals:
    
public slots:
    void slotAddChoosenContact();
    void slotAddContactWithDoubleClicked(QListWidgetItem*);
};

#endif // ADDFRIENDWINDOW_H
