#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
class MyPushButton: public QPushButton
{
    Q_OBJECT

    public:
        MyPushButton(QString text, int id, /*QString user = "", QString pwd = "",*/ QWidget *parent = 0)
            : QPushButton(text, parent),id(id)/*, name(user), pwd(pwd)*/{};

        //QString name;
        //QString pwd;
        int id;

};

#endif // MYPUSHBUTTON_H
