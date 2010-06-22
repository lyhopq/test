#ifndef TOBACCO_H
#define TOBACCO_H

#include <QString>
#include <QSqlQuery>
#include <QMap>
#include <QStringList>
#include <QSettings>
#include <QBitArray>

#include "userdialog.h"
#include "pwddialog.h"

class MyDB
{
    public:
        MyDB();
        QSqlQuery query;

        static bool createConn();
};

class sysVal
{
    public:
        enum STATUS{END = -1, RUN, STOP, PAUSE};
        sysVal();
        ~sysVal();

        void userHand(int type, int id, QStringList &d);


        MyDB *dm;     // 数据库管理

        //QString user;
        //QString pwd;
        bool isDBok;

        bool getUser(); // 从数据库检索用户信息

        int userID;    // 当前用户ID
        int bigID;     // 用户已用最大ID
        int userCount; // 用户总数
        //int dirty;   // 用户数改变
        QBitArray dirty;   // 更新该标志
        //QMap<QString, QString> userMap;
        QMap<int, QStringList> userMap;     // 用户信息
        QStringList userName;    // 用户名

        QSettings *sets;
        QMap<int, QString> cfg;  // 参数
        void getSettings();
        void saveSettings();

        int xq;  // 星期几
        QStringList week;
        bool getBC(int w, QStringList &list);     // 获取班次设置
        bool saveBC(int w, QStringList &list);     // 保存班次设置

        // 每小时报告
        QMap<int, int> map[8];
        STATUS status; // 当前状态
        int stime; // 开始时间X2
        void updateMap();

        QString getCS(int id, int n); // 获得用户参数设置
        bool saveCS(int id, int n, QString &); // 获得用户参数设置


};



#endif // TOBACCO_H
