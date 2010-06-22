#ifndef CLOCKLABEL_H
#define CLOCKLABEL_H

#include <QLabel>

class ClockLabel: public QLabel
{
    Q_OBJECT

    public:
        ClockLabel(QWidget *parent = 0);

    protected:
        void timerEvent(QTimerEvent *);
    private:
        inline QString tranWeek(int d);
};

#endif // CLOCKLABEL_H
