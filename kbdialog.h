#ifndef KBDIALOG_H
#define KBDIALOG_H

#include <QDialog>
#include <QFile>

enum inputMethod{english,chinese};

namespace Ui {
    class KBDialog;
}

class KBDialog : public QDialog {
    Q_OBJECT
public:
    KBDialog(QWidget *parent = 0);
    ~KBDialog();

    QString getRes() const {return res;};



protected:
    void changeEvent(QEvent *e);
    virtual bool eventFilter(QObject *, QEvent *);

private:
    Ui::KBDialog *ui;
    QString res;

    bool isShiftDown;
    bool findNothing;

    inputMethod method;

    int pageCount;
    int currentPage;
    //int currentZh;

    QKeyEvent* transKey(QPushButton *);
    QString transPy(QString pyInput);    // 根据输入拼音查询汉

    QString py;

    QFile file;     // 拼音汉字编码文件
    QFile out;    // 查询结果
    //QStringList zhlist; // 汉字列表

    QRegExp regExp;     // 用正则表达式查询汉字

    void changKeyboard();    // 更换大小写布

    void showPage(int index);     // 显示汉字选择列表

    void clearZh();

private slots:
    void on_pb_change_clicked(); // 更换输入法类型
    void on_pb_shift_clicked();    // 大小写转换

    void on_pb_h1_clicked();    // 汉字选择一
    void on_pb_h2_clicked();
    void on_pb_h3_clicked();
    void on_pb_h4_clicked();
    void on_pb_h5_clicked();
    void on_pb_h6_clicked();
    void on_pb_h7_clicked();
    void on_pb_h8_clicked();

    void on_pb_per_clicked();    // 上一页
    void on_pb_next_clicked();

    void on_pb_ok_clicked();
    void on_pb_cancel_clicked();

};

#endif // KBDIALOG_H
