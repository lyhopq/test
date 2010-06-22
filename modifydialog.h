#ifndef MODIFYDIALOG_H
#define MODIFYDIALOG_H

#include <QDialog>

namespace Ui {
    class modifyDialog;
}

class modifyDialog : public QDialog {
    Q_OBJECT
public:
    modifyDialog(QWidget *parent = 0);
    ~modifyDialog();

    void setUser(QString &);

protected:
    void changeEvent(QEvent *e);

    virtual bool eventFilter(QObject *, QEvent *);

private:
    Ui::modifyDialog *ui;

private slots:
    void on_pb_ok_clicked();
};

#endif // MODIFYDIALOG_H
