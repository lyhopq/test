#include <QPainter>
#include <QDebug>

#include "mystatus.h"
#include "tobacco.h"

extern sysVal *sysval;

MyStatus::MyStatus(QWidget *parent)
    : QFrame(parent), w(132), h(50)
{
    //flag = true;
    count = 0;

    /*
    map = new QMap<int, int>*[8];

    for(int i = 0; i < 8; i++)
    {
        map[i] = new QMap<int, int>;
    }
    */

    //sysval->map[7][40] = sysVal::RUN;
    //sysval->map[7][80] = sysVal::PAUSE;

    startTimer(30000); // 30s
}

void MyStatus::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    painter.setWindow(0, 0, w, h);
    //painter.scale()

    //painter.save();

    painter.setPen(Qt::white);
    int d1 = 6;
    int d2 = 20;

    painter.drawLine(0, 0, 0, h);
    painter.drawLine(w, 0, w, h);
    painter.drawLine(0, 0, w, 0);
    painter.drawLine(0, h, w, h);

    for(int i = 0; i < 7; i++)
    {
        painter.drawLine(d1, 0, d1, h);

        d1 += d2;
    }

    //painter.restore();


    /*
    QPen pen;
    pen.setWidth(5);
    pen.setColor(Qt::green);
    painter.setPen(pen);
    */

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::green);

    d1 = 6;
    painter.translate(QPoint(d1, h-2));

    painter.save();


    // 过去7小时
    int k1, k2, v1, v2;
    //if(flag)
    {
        for(int j = 0; j < 7; j++)
        {
            QMapIterator<int, int> i(sysval->map[j]);

            i.next();
            k1 = i.key();
            v1 = i.value();

            painter.translate(QPoint(0, -6));
            while (i.hasNext())
            {
                i.next();
                k2 = i.key();
                v2 = i.value();

                changeColor(v1, j+1, painter);
                painter.drawRect(QRect(k1, 0, k2-k1, -4));

                k1 = k2;
                v1 = v2;
            }

        }
        //flag = false;
    }


    // 当前小时
    painter.restore();

    QMapIterator<int, int> i(sysval->map[7]);
    i.next();
    k1 = i.key(); v1 = i.value();
    while (i.hasNext())
    {
        i.next();
        k2 = i.key();
        v2 = i.value();

        changeColor(v1, 1, painter);
        painter.drawRect(QRect(k1, 0, k2-k1, -4));

        k1 = k2;
        v1 = v2;
    }

    changeColor(sysval->status, 1, painter);
    painter.drawRect(QRect(k1, 0, count, -4));
}

void MyStatus::changeColor(int t, int level, QPainter &p)
{
    QColor c;
    switch(t)
    {
        case sysVal::RUN:
            c = QColor(Qt::green);
            break;
        case sysVal::STOP:
            p.setBrush(Qt::NoBrush);
            return;
            break;
        case sysVal::PAUSE:
            c = QColor(Qt::red);
            break;
    }

    c = c.darker(level*108);
    p.setBrush(c);
}

/*
void MyStatus::showEvent(QShowEvent *)
{
    flag = true;
    update();
}
*/

void MyStatus::timerEvent(QTimerEvent *)
{
    //...
    //
    count++;
    /*
    if(count%2)
        flag = true;
    */
    //qDebug() << count;

    if(count + sysval->stime >= 121)
    {
        sysval->map[7][120]  = sysVal::END;
        sysval->updateMap();
        count = 1;
        sysval->stime = 0;

        //flag = true;
    }

    repaint();
}


