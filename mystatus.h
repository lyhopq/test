#ifndef MYSTATUS_H
#define MYSTATUS_H

#include <QFrame>
#include <QMap>

class MyStatus: public QFrame
{
    Q_OBJECT

    public:
        MyStatus(QWidget *parent = 0);

    protected:
        void paintEvent(QPaintEvent *e);
        //void showEvent(QShowEvent *e);
        void timerEvent(QTimerEvent *e);

    private:

        //bool flag;
        const int w;
        const int h;

        int count; // 最后一小时计数

        void changeColor(int t, int level, QPainter &p);
};


#endif // MYSTATUS_H
