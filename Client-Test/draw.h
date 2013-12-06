#ifndef DRAW_H
#define DRAW_H

#include <QtGui/QWidget>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QDialog>
#include "myclient.h"

class MyClient;

struct MyQPoint
{
    QPoint point;
    bool repaintWithNext;
    MyQPoint(QPoint thisPoint) {
        point = thisPoint;
        repaintWithNext=true;
    }
    MyQPoint()
    {
        repaintWithNext = false;
    }
    MyQPoint(int x, int y)
    {
        point = QPoint(x, y);
        repaintWithNext = true;
    }
};

class PaintWidget : public QDialog
{
    Q_OBJECT

public:
    QList<MyQPoint> gettingPoint;
    QList<MyQPoint> toSendPoints;
    PaintWidget(QWidget *parent = 0, MyClient* socket = 0 );
    ~PaintWidget();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
private:
    QList<MyQPoint> mDrawBuffer;

    bool mDrawMode;
    bool newLine;
    MyClient* client;
};

#endif // DRAW_H
