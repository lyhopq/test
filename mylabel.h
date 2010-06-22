/*
 * =====================================================================================
 *
 *       Filename:  mylabel.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010年05月31日 17时28分00秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 * =====================================================================================
 */

#ifndef  MYLABEL_H
#define  MYLABEL_H

#include <QFrame>
#include <QLineEdit>

class QLabel;

class MyLabel: public QFrame
{
    Q_OBJECT

    public:
        MyLabel(const QString &_l1, const QString &_l2 = QString("??")
            , const QString &_l3 = QString(""), QWidget *parent = 0);

        void setV(const QString &s) { l4->setText(s); };
        QString getV() const { return l4->text(); };

        void changStatus(bool on);

        void setNum(int _num) { num = _num; };

    private:
        QLabel *l0;
        QLabel *l1;
        QLabel *l2;
        QLabel *l3;
        QLineEdit *l4;

        int num;

    protected:
        bool eventFilter(QObject *obj, QEvent *event);

    signals:
        void clicked(int);
};


#endif   /* ----- #ifndef MYLABEL_H  ----- */

