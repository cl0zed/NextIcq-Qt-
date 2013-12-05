#include "addfriendwindow.h"
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
addFriendWindow::addFriendWindow(mainWidget *parent, MyClient *socket) :
    QDialog(parent)
{
    this->setWindowTitle("Add Friend");
    parentMainWidget = parent;

    socket->sendContactListRequest();

    socket->waitMessage();

    allContacts = new QListWidget;

    foreach(QString str, socket->contacts)
    {
        qDebug() << str;
        allContacts->addItem(str);
    }
    socket->contacts.clear();
    QPushButton* addFriend = new QPushButton("&Add");
    QPushButton* close = new QPushButton("&Close");

    connect(addFriend, SIGNAL(clicked()), SLOT(slotAddChoosenContact()));
    connect(close, SIGNAL(clicked()), this, SLOT(close()));
    connect(allContacts, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(slotAddContactWithDoubleClicked(QListWidgetItem*)));

    QHBoxLayout* hlay = new QHBoxLayout;

    QVBoxLayout* vlay = new QVBoxLayout;

    hlay->addWidget(addFriend);
    hlay->addWidget(close);

    vlay->addWidget(allContacts);

    vlay->addLayout(hlay);

    this->setLayout(vlay);
}

void addFriendWindow::slotAddChoosenContact()
{
    const QString newItem = allContacts -> currentItem()->text();
    addCurrentItem(newItem);
}

void addFriendWindow::slotAddContactWithDoubleClicked(QListWidgetItem *currentItem)
{
    const QString newItem = currentItem->text();
    addCurrentItem(newItem);
}
void addFriendWindow::addCurrentItem(const QString& newItem)
{
    if ( parentMainWidget->contacts->findItems(newItem, Qt::MatchContains).count() == 0)
    {
        parentMainWidget->contacts->addItem(allContacts->currentItem()->text());
    }
}
