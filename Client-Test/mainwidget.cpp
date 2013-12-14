#include "mainwidget.h"
#include "addfriendwindow.h"
#include "dialogwindow.h"
#include <QAction>
#include <QMenu>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
void mainWidget::createMenu()
{
    contextMenu = new QMenu(this);

    QAction* startDialogAction = new QAction("Start Dialog", 0);
    QAction* deleteUser = new QAction("Delete from Contact List", 0);
    QAction* startPaint = new QAction("Start Paint", 0);

    contextMenu->addAction(startDialogAction);
    contextMenu->addAction(deleteUser);
    contextMenu->addAction(startPaint);


    connect(contacts, SIGNAL(customContextMenuRequested(QPoint)), SLOT(slotMenuRequested(QPoint)));
    connect(startDialogAction, SIGNAL(triggered()), SLOT(slotDialogStart()));
    connect(deleteUser, SIGNAL(triggered()), SLOT(slotDeleteFromContactList()));
    connect(startPaint, SIGNAL(triggered()), SLOT(slotStartPaint()));
}

void mainWidget::writeSettings()
{
    settings.beginGroup("/Settings");
    settings.setValue("/count", contacts->count());
    settings.setValue("/UserName", client->takeMyName() );
    for (int i = 0; i < contacts->count(); ++i)
    {
        QString str = "/contact" + QString::number(i);
        settings.setValue(str, contacts->item(i)->text());
    }
    settings.endGroup();
}

void mainWidget::readSettings()
{
    settings.beginGroup("/Settings");
    int numberOfContacts = settings.value("/count", 0).toInt();
    QString str = settings.value("/UserName", "").toString();

    client->setMyName(str);
    if (str != "") client->NewRegiter();
    for (int i = 0; i < numberOfContacts; ++i)
    {
        QString str = settings.value("/contact" + QString::number(i), "").toString();
        QListWidgetItem* newItem = new QListWidgetItem(str);
        contacts->addItem(newItem);
    }
    settings.endGroup();

}

void mainWidget::closeEvent(QCloseEvent *)
{
    client->sendMessageToDelete();
    client->waitSendMessage();
    writeSettings();
}

mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent)
{
    dialog = 0;
    this->setWindowTitle("Icq");
#ifdef QT_NO_DEBUG
    QFile file("serverAddress.txt");
    qDebug() << "i'm here";
#else
    QFile file("../NextIcq/Client-Test/serverAddress.txt");
#endif
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to Open";
        return;
    }

    QTextStream stream(&file);
    QString IP;
    IP = file.readLine();
    qDebug() <<"IP" << IP;
    client = new MyClient(IP , 2324, this);

    this->setFixedSize(200, 300);
    contacts = new QListWidget;
    QPushButton* addFriend = new QPushButton("&Add friend");
    QPushButton* startDialog = new QPushButton("&Start dialog");

    createMenu();
    readSettings();

    QHBoxLayout* nameLay = new QHBoxLayout;

    if (client->takeMyName() == "" || client->takeMyName() == 0)
    {
        setName = new QLineEdit;
        QPushButton* set = new QPushButton("&Use this name");
        connect(set, SIGNAL(clicked()), SLOT(slotSetName()));
        nameLay->addWidget(setName);
        nameLay->addWidget(set);
    }
    connect(addFriend, SIGNAL(clicked()), SLOT(slotAddFriend()));
    connect(startDialog, SIGNAL(clicked()), SLOT(slotDialogStart()));
    connect(contacts, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(slotDialogStart(QListWidgetItem*)));

    contacts->setContextMenuPolicy(Qt::CustomContextMenu);

    QVBoxLayout* vlay = new QVBoxLayout;

    QHBoxLayout* buttonsLay = new QHBoxLayout;

    buttonsLay->addWidget(addFriend);
    buttonsLay->addWidget(startDialog);

    vlay->addWidget(contacts);
    vlay->addLayout(buttonsLay);
    if (nameLay->count() != 0)
        vlay->addLayout(nameLay);
    this->setLayout(vlay);

}

void mainWidget::slotAddFriend()
{
    addFriendWindow* friendWindow = new addFriendWindow(this, client);
    friendWindow->setModal(true);
    friendWindow->show();
}

void mainWidget::slotDialogStart()
{
    if ( contacts->count() != 0 &&  ( dialog == 0 || client->takeClientName() != contacts->currentItem()->text() ) )
    {
        dialog = new dialogWindow(this, client, contacts->currentItem()->text());
        //client->takeDialog(dialog);
        dialog ->setModal(true);
        dialog->show();
    }
    else if (dialog != 0 && client->takeClientName() == contacts->currentItem()->text() )
    {
        dialog->show();
    }
}

void mainWidget::slotDialogStart(QListWidgetItem *item)
{
    if (contacts->count() != 0 && (dialog == 0 || client->takeClientName() != item->text()) )
    {
        dialog = new dialogWindow(this, client, item->text());
        dialog ->setModal(true);
        dialog->show();
    }
    else if (dialog != 0 && client->takeClientName() ==  item->text() )
    {
        dialog->show();
    }
}

void mainWidget::slotMenuRequested(QPoint pos)
{
    QListWidgetItem* item = contacts->itemAt(pos);
    if (item != 0)
    {
        contextMenu->exec(contacts->mapToGlobal(pos));
    }
}

void mainWidget::slotDeleteFromContactList()
{
    contacts->takeItem(contacts->currentRow());
}

void mainWidget::slotSetName()
{
   client->setMyName(setName->text());
   setName->setReadOnly(true);
   client->NewRegiter();
}

void mainWidget::slotStartPaint()
{
    paint = new PaintWidget(this, client);
    paint->setModal(true);
    client->setSenderName(contacts->currentItem()->text());
    paint->show();
}
