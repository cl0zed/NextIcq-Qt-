#include "draw.h"
#include <QtCore/QLine>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

PaintWidget::PaintWidget(QWidget *parent)
    : QWidget(parent)
{
    mDrawMode = false;
}

PaintWidget::~PaintWidget()
{

}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mDrawMode = true;
        mDrawBuffer.append((MyQPoint(event->pos())));
        event->accept();
    }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mDrawMode = false;
        mDrawBuffer.append(MyQPoint(event->pos()));
        mDrawBuffer.last().repaintWithNext=false;
        this->update();
        event->accept();
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!mDrawMode) return;
    mDrawBuffer.append(MyQPoint(event->pos()));
    this->update();
    event->accept();
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    if (mDrawBuffer.size() < 2) return;
    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 3 ));
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
