#ifndef MYPB_H
#define MYPB_H

#include <QFrame>

class QTimer;

class MyPb: public QFrame
{
    Q_OBJECT

    public:
        MyPb(QWidget *parent = 0);

        //bool getAngle() const { return angle; };
        //void changeAngle() { angle = !angle; update(); };

    protected:
        void paintEvent(QPaintEvent *event);
        //void mousePressEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);

    private:
        QRect rect;
        bool isMouseOn;
        bool isClicked;

        QTimer *timer;

    public slots:
        void changeAngle() { /*angle = !angle;*/ update(); };

    private slots:
        void timeout();

    signals:
        void clicked();
};

#endif // MYPB_H
