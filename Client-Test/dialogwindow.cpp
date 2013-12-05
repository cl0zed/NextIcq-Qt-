#include "dialogwindow.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QTime>
QWidget *dialogWindow::createTab()
{
    QWidget* wdgt = new QWidget;
    allMessages = new QTextEdit;

    currentMessage = new QTextEdit;

    QPushButton* send = new QPushButton("&Send");

    connect(send, SIGNAL(clicked()), SLOT(slotSend()));

    QHBoxLayout* hlay = new QHBoxLayout;
    QVBoxLayout* vlay = new QVBoxLayout;

    hlay->addWidget(currentMessage);
    hlay->addWidget(send);

    vlay->addWidget(allMessages);
    vlay->addLayout(hlay);

    wdgt->setLayout(vlay);
    return wdgt;

}

/*void dialogWindow::closeEvent(QCloseEvent *)
{
    this->hide();
}*/

dialogWindow::dialogWindow(mainWidget *parent, MyClient* socket, const QString& str) :
    QDialog(parent), clientName(str)
{
    this->setWindowTitle("Dialogs");
    client = socket;

    mainTab = new QTabWidget;

    mainTab->addTab(createTab(), str);

    QHBoxLayout* hlay = new QHBoxLayout;

    hlay->addWidget(mainTab);

    this->setLayout(hlay);
}

void dialogWindow::slotSend()
{
    client->setSenderName(clientName);

    client->setMessage(currentMessage->toPlainText());

    allMessages->append("Send >> [ " + QTime::currentTime().toString() +
                        " ] " + client->takeMyName() + " : " +
                        currentMessage->toPlainText());

    currentMessage->setText("");

    client->send();

    qDebug() << "send end";

}
