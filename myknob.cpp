#include <QPainter>
#include <QFontMetrics>
#include <QRect>
#include <QMouseEvent>

#include "myknob.h"

MyKnob::MyKnob(QString _l1, QString _l2, QWidget *parent)
    : QFrame(parent), l1(_l1), l2(_l2)
{
    angle = 135.0;
    //resize(100, 100);

    isMouseOn = false;

    setMinimumHeight(60);
    setMouseTracking(true);
}

void MyKnob::paintEvent(QPaintEvent *)
{
    QFontMetrics fm(font());
    static const int w = fm.width(l1);
    static const int h = fm.height();

    const int p = qMax(width()/4-w/2, 10);
    const int r = qMin(width(), height()-h)/2 - 3;


    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawText(p, h, l1);
    painter.drawText(width()-p-w, h, l2);

    //painter.setWindow(QRect(0,0,100,100));

    const QPointF center(QPoint(width()/2, (height()+h)/2));

    painter.drawEllipse(center, r, r);

    rect = QRect(-r+center.x(), -r+center.y(), r*2, r*2);

    if(isMouseOn)
    {
        painter.setBrush(QColor(Qt::green).light(120));
        painter.drawRect(rect);
    }

    //painter.save();


    /***
    QRadialGradient gradient(center, r);
    gradient.setColorAt(0, QColor(Qt::gray));
    gradient.setColorAt(1.0, QColor(Qt::lightGray));
    QBrush brush(gradient);
    painter.setBrush(brush);
    ***/
    painter.setBrush(QColor(Qt::lightGray));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, r-6, r-6);


    //painter.restore();
    painter.translate(center);
    qreal a;
    if(angle)
        a = 45;
    else
        a = 135;
    painter.rotate(-a);

    const int width = qMax(14, r/5);
    const int length = qMax(15, r/6);
    painter.setPen(QColor(Qt::white));
    painter.setBrush(Qt::NoBrush);

    painter.drawRect(QRect(-(r+5), -width/2, r*2+5, width));

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(Qt::gray));
    painter.drawRect(QRect(r-length, -width/2, length, width));

    painter.setBrush(QColor(Qt::lightGray));
    painter.drawRect(QRect(-(r+3), -width/2, length-5, width));

    painter.setBrush(QColor(Qt::red));
    painter.drawRect(QRect(r-length/2-3, -width/4, 5, width/2));
    //painter.drawPoint(QPoint(r-6, 0));

    //painter.restore();
}

void MyKnob::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton && rect.contains(e->pos()))
    {
        emit clicked();
        isMouseOn = false;
        update();
    }
}

void MyKnob::mouseMoveEvent(QMouseEvent *e)
{
    if(rect.contains(e->pos()))
    {
        isMouseOn = true;
    }
    else
        isMouseOn = false;

    update();
}
