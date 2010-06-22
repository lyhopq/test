#ifndef PWDDIALOG_H
#define PWDDIALOG_H

#include <QDialog>

namespace Ui {
    class pwdDialog;
}

class pwdDialog : public QDialog {
    Q_OBJECT
public:
    pwdDialog(QWidget *parent = 0);
    ~pwdDialog();

    QString getPwd() const {return res;};



protected:
    void changeEvent(QEvent *e);

private:
    Ui::pwdDialog *ui;
    QString res;

private slots:
    void getKey();
    void on_okButton_clicked();
    void on_cancelButton_clicked();
};

#endif // PWDDIALOG_H
