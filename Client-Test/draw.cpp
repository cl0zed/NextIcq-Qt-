#include "draw.h"
#include <QtCore/QLine>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QPalette>
#include <QBrush>
#include <QPen>
PaintWidget::PaintWidget(QWidget *parent, MyClient *socket)
    : QDialog(parent)
{
    client = socket;
    mDrawMode = false;
    this->setPalette(QPalette(Qt::white));
    this->setUpdatesEnabled(true);
}

PaintWidget::~PaintWidget()
{

}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mDrawMode = true;
        mDrawBuffer.append(MyQPoint(event->x(), event->y()));
        toSendPoints.append(MyQPoint(event->x(), event->y()));
        event->accept();
    }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mDrawMode = false;
        mDrawBuffer.append(MyQPoint(event->x(), event->y()));
        toSendPoints.append(MyQPoint(event->x(), event->y()));

        mDrawBuffer.last().repaintWithNext= false;
        toSendPoints.last().repaintWithNext = false;

        client->sendPaintedPoints(toSendPoints, true);

        this->update();
        event->accept();
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!mDrawMode) return;
    mDrawBuffer.append(MyQPoint(event->x(), event->y()));
    toSendPoints.append(MyQPoint(event->x(), event->y()));

    if (toSendPoints.count() >= 159) client->sendPaintedPoints(toSendPoints, false);
    qDebug() << toSendPoints.count();
    this->update();
    event->accept();
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    if (mDrawBuffer.size() < 2 && gettingPoint.size() < 2) return;
    QPainter painter(this);
    if ( !gettingPoint.isEmpty() )
    {
        //qDebug() << gettingPoint.count();
        painter.setPen(QPen(Qt::green, 3));
        QList<MyQPoint>::const_iterator it = gettingPoint.begin();
        MyQPoint start = *it;
        while (it != gettingPoint.end())
        {
            MyQPoint end = *it;
            if (start.repaintWithNext)
                painter.drawLine(start.point, end.point);
            start = end;
            ++it;
        }
    }
    if (!mDrawBuffer.isEmpty())
    {
        painter.setPen(QPen(Qt::red, 3));
        QList<MyQPoint>::const_iterator it = mDrawBuffer.begin();
        MyQPoint start = *it;
        while(it != mDrawBuffer.end()) {
            MyQPoint end = *it;
            if (start.repaintWithNext)
                painter.drawLine(start.point, end.point);
            start = end;
            it++;
        }
    }
}
