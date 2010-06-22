#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
#include <QVBoxLayout>

#include "mypushbutton.h"


namespace Ui {
    class userDialog;
}

class userDialog : public QDialog {
    Q_OBJECT
public:
    userDialog(QWidget *parent = 0);
    ~userDialog();

    void updateUser();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::userDialog *ui;
    int c1;
    QList<MyPushButton *> allButtons;



    void buttonEnable();

private slots:
    void previous();
    void next();
    void getPwd();
};

#endif // USERDIALOG_H
