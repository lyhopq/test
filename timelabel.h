#ifndef TIMELABEL_H
#define TIMELABEL_H

#include <QFrame>
#include <QLabel>

class TimeLabel: public QFrame
{
    Q_OBJECT

    public:
        TimeLabel(QWidget *parent = 0);
        void setColor(bool on = true, const QColor &bc = QColor(255, 255, 191));
        void setNum(int _num) { num = _num; };

        void setTime(const QString &v) { label->setText(v); };
        QString getTime() const { return label->text(); };

    protected:
        bool eventFilter(QObject *obj, QEvent *e);

    private:
        QLabel *label;
        int num;
    signals:
        void tClicked(int);
};

#endif // TIMELABEL_H
