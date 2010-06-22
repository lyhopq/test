#ifndef MYKNOB_H
#define MYKNOB_H


#include <QFrame>
#include <QString>

class MyKnob: public QFrame
{
    Q_OBJECT

    public:
        MyKnob(QString l1, QString l2, QWidget *parent = 0);

        bool getAngle() const { return angle; };
        void setAngle(bool on) { angle = on; update(); };

    protected:
        void paintEvent(QPaintEvent *event);
        void mousePressEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);

    private:
        QString l1;
        QString l2;
        bool angle;

        QRect rect;
        bool isMouseOn;

    /***
    public slots:
        void changeAngle() { angle = !angle; update(); };
    ***/

    signals:
        void clicked();
};

#endif // MYKNOB_H
