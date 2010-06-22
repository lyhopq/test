#include <QTimer>
#include <QPainter>
#include <QMouseEvent>

#include "mypb.h"

MyPb::MyPb(QWidget *parent)
    : QFrame(parent)
{
    //resize(100, 100);

    isMouseOn = isClicked = false;

    setMouseTracking(true);

    timer = new QTimer(this);
    timer->setSingleShot(true);

    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

}

void MyPb::paintEvent(QPaintEvent *)
{
    const QPointF center(QPoint(width()/2, height()/2));
    const int r = qMin(width(), height())/3;
    const double h = 0.866*r + 4;

    const QPointF points[8] =
    {
        QPointF(-r/2, -h),
        QPointF(r/2, -h),
        QPointF(h, -r/2),
        QPointF(h, r/2),
        QPointF(r/2, h),
        QPointF(-r/2, h),
        QPointF(-h, r/2),
        QPointF(-h, -r/2)
    };

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    rect = QRect(-r*1.2+center.x(), -h*1.2+center.y(), h*2.2, h*2.4);
    if(isMouseOn)
    {
        painter.setBrush(QColor(Qt::green).light(120));
        painter.drawRect(rect);
    }

    painter.translate(center);



    QLinearGradient linear(-r/2, -h, r/2, h);
    linear.setColorAt(0.0, Qt::lightGray);
    linear.setColorAt(1.0, Qt::black);
    painter.setBrush(QBrush(linear));
    painter.drawConvexPolygon(points,8);



    linear.setColorAt(0.0, QColor(Qt::darkGray).dark(200));
    linear.setColorAt(1.0, Qt::lightGray);
    painter.setBrush(QBrush(linear));
    painter.drawEllipse(QPointF(0,0), r*0.8, r*0.8);

    if(isClicked)
    {
        QRadialGradient radial(0, 0, r*0.6);
        radial.setColorAt(0, Qt::red);
        radial.setColorAt(1.0, Qt::green);
        painter.setBrush(radial);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPointF(0,0), r*0.6, r*0.6);

        timer->start(1000); // 1s
    }
    else
    {
        painter.setBrush(Qt::red);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPointF(0,0), r*0.6, r*0.6);
    }
}

//void MyPb::mousePressEvent(QMouseEvent *e)
void MyPb::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton && rect.contains(e->pos()))
    {
        isClicked = true;
        isMouseOn = false;
        update();
        emit clicked();
    }
}

void MyPb::mouseMoveEvent(QMouseEvent *e)
{
    if(rect.contains(e->pos()))
    {
        isMouseOn = true;
    }
    else
        isMouseOn = false;

    update();
}

void MyPb::timeout()
{
    isClicked = false;
    isMouseOn = false;
    update();
}
