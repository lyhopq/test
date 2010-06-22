#ifndef INDEX_H
#define INDEX_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QButtonGroup>
#include <Qwt/qwt_slider.h>


namespace Ui {
    class Index;
}

class MyKnob;
class MyPb;
class MyLabel;
class TimeLabel;
class Plot2;

class Index : public QWidget {
    Q_OBJECT
public:
    Index(QWidget *parent = 0);
    ~Index();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Index *ui;


    // 用户管理
    int c; // 用户列表的当前选择位置
    int sID; // select user ID
    void updateUser(); // 更新用户
    void userButtonEnable();


    // 功能设置
    void setGNSZ();
    MyPb   *p1;
    MyPb   *p2;
    MyPb   *p3;
    MyPb   *p4;

    MyKnob *c1;
    MyKnob *c2;
    MyKnob *c3;
    MyKnob *c4;

    // 参数设定
    void setCSSD();
    MyLabel *mylabel[32];
    void changeStatus();

    // 重量报告
    void setZLBG();

    // 班次设置
    void setBCSZ();
    TimeLabel *tLabel[20];
    int currNum; // 当前星期
    void saveBC(int week);

    // 生产数据
    void setSCSJ();

    // 运行监视
    void setYXJS();
    QwtSlider *slider;
    QLabel *v1;
    QLabel *v2;

    Plot2 *plot2;
    QButtonGroup *bg;
    QPushButton *b20;
    QPushButton *b50;
    QPushButton *b100;
    QLabel *t1;
    QLabel *t2;
    QLabel *v3;
    QLabel *v4;

    // 运行状态
    void setYXZT();

    // 系统设置
    void setXTSZ();
    QTimer *bc_timer;
    void display_bc(QStringList &list);
    int c_banci; // 当前班次
    int l_banci; // 上一班次
    QStringList bc_list; // 当前班次设置
    void updateBC();
    void note_curr_banci();
    void dis_note_last_banci();
    void seek_curr_banci(); // 寻找当前班次
    //QMap<int, int> time_map; // 记录班次间隔----> 用于定时


public slots:
    // 用户管理
    void userInfo();
    void on_pb_per_clicked();
    void on_pb_next_clicked();

    void on_buttonGroupQX_buttonPressed(int);   // 全选
    void on_buttonGroupQCX_buttonPressed(int);  // 全撤销

    void load_canshu(int n, QWidget *);  // 加载权限管理的初始参数设置
    void save_canshu(int n, QWidget *);   // 保存权限管理的参数设置


    void on_syssetButton_clicked();
    void on_gravityButton_clicked();
    void on_loginButton_clicked();
    void on_logoutButton_clicked();
    void on_statusButton_clicked();
    void on_dataButton_clicked();
    void on_supportButton_clicked();
    void on_exitButton_clicked();

    void on_pb_bc_clicked();
    void on_pb_yh_clicked();
    void on_pb_xg_clicked();
    void on_pb_hb_clicked();

    void on_pb_js_clicked();
    void on_pb_cs_clicked();

    void on_pb_ctrl_clicked();
    void on_pb_tl_clicked();

    void on_pb_close1_clicked();

    void on_backButton1_clicked();
    void on_backButton3_clicked();
    void on_backButton4_clicked();
    void on_backButton6_clicked();
    void on_backButton8_clicked();

    void on_nameEdit_getFocus();

    void on_pb_ok_clicked();
    void on_pb_cancel_clicked();
    void on_pb_ok2_clicked();
    void on_pb_cancel2_clicked();


    void on_buttonGroup_buttonPressed(int);
    void on_buttonGroup2_buttonPressed(int);
    void on_buttonGroup3_buttonPressed(int);
    void on_buttonGroup4_buttonPressed(int);
    void on_buttonGroup5_buttonPressed(int);
    void on_buttonGroup6_buttonPressed(int);
    void on_buttonGroupTZ_buttonPressed(int);

    void on_useraddPb_clicked();
    void on_userdelPb_clicked();
    void on_pb_userSave_clicked();
    void on_pb_userCancel_clicked();

    void lineEdit_name_getFocus();
    void lineEdit_pri_getFocus();
    void lineEdit_time_getFocus();

    // 功能设置
    /**
    void plClicked();
    void p2Clicked();
    void p3Clicked();
    void p4Clicked();
    ***/

    void clClicked();
    void c2Clicked();
    void c3Clicked();
    void c4Clicked();

    // 参数设定
    void clicked(int num);
    void getVal(int num);

    // 班次设置
    void tClicked(int num);
    void on_buttonGroupXQ_buttonPressed(int);

    // 生产数据
    void on_buttonGroupTJFX_buttonPressed(int);

    // 运行监视
    void onbg_buttonPressed(int);

    // 系统设置
    void bc_timer_timerout();
};

#endif // INDEX_H
