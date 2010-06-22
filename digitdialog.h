#ifndef DIGITDIALOG_H
#define DIGITDIALOG_H

#include <QDialog>

namespace Ui {
    class digitDialog;
}

class QRegExp;

class digitDialog : public QDialog {
    Q_OBJECT
public:
    digitDialog(const QString& _l1, const QString& _l2, /*int type,*/ QWidget *parent = 0);
    ~digitDialog();

    QString getRes() const { return res; };
    void changeLabel(const QString &_l1, const QString &_l2);

    void setType(int type);
    bool isFitting();

protected:
    void changeEvent(QEvent *e);
    virtual bool eventFilter(QObject *, QEvent *);

private:
    Ui::digitDialog *ui;

    QString l1;
    QString l2;
    int type;

    QString res;

    const QRegExp *reg;

private slots:
    void on_pb_cancel_clicked();
    void on_pb_ok_clicked();

};

#endif // DIGITDIALOG_H
