#ifndef DRAW_H
#define DRAW_H

#include <QtGui/QWidget>
#include <QtCore/QList>
#include <QtCore/QPoint>
struct MyQPoint
{
    QPoint point;
    bool repaintWithNext;
    MyQPoint(QPoint thisPoint) {
        point = thisPoint;
        repaintWithNext=true;
    }
};

class PaintWidget : public QWidget
{
    Q_OBJECT

public:
    PaintWidget(QWidget *parent = 0);
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
};

#endif // DRAW_H
