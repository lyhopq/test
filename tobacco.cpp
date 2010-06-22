    #include <QMessageBox>
    #include <QApplication>
    #include <QTime>
    #include <QString>
    #include <QDebug>

    #include "tobacco.h"

    extern MyDB *dm;


    // 数据库相关
    MyDB::MyDB()
    {
    }


    bool MyDB::createConn()
    {
        bool conn;

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("tobacco.db");

        conn = db.open();


        if(!conn)
        {
            QMessageBox::critical(0, qApp->tr("Cannot open database"),
                qApp->tr("Unable to establish a database connection."),
                QMessageBox::Cancel);
            return false;
        }
        return true;
    }

    // sysVal


    sysVal::sysVal():userID(-1),bigID(-1), userCount(-1)
    {
       isDBok = MyDB::createConn();
       if(isDBok )
       {
           dm = new MyDB;
           getUser();
       }

       dirty.resize(10);
       //dirty.fill(true);
       dirty.fill(false);

       QString file = "tobacco.ini";
       sets = new QSettings(file, QSettings::IniFormat);
       getSettings();

       week << "Monday" << "Tuesday" << "Wednesday" << "Thursday"
               << "Friday" << "Saturday" << "Sunday";

        for(int i = 0; i < 7; i++)
        {
            map[i][0] = STOP;
            map[i][120] = END;
        }

        xq = QDate::currentDate().dayOfWeek(); // 当前星期

        QTime t = QTime::currentTime();
        stime = t.minute()*2 + t.second()/30;

        //qDebug() << t << stime;
        map[7][0] = STOP;
        map[7][stime] = PAUSE;

        status = PAUSE;
    }

    sysVal::~sysVal()
    {
        //saveSettings();
        delete dm;
    }

    bool sysVal::getUser()
    {
        /***
        bool test = dm->query.exec("select name,pwd from user where name='Administratorlyh';");
        dm->query.last();
        if(test)
            userMap["管理员"] = dm->query.value(1).toString();
        qDebug() << dm->query.value(1).toString() << userMap["管理员"];
        ***/
        //bool test = dm->query.exec("select name,pwd from user where name!='Administratorlyh';");
        bool test = dm->query.exec("select id,name,pwd,pri,time from user order by id;");

        QStringList tmp;
        while(dm->query.next())
        {
            tmp << dm->query.value(1).toString()
                << dm->query.value(2).toString()
                << dm->query.value(3).toString()
                << dm->query.value(4).toString();

            userMap[dm->query.value(0).toInt()] = tmp;

            userName << dm->query.value(1).toString();

            //qDebug() << tmp << dm->query.value(0).toInt();
            tmp.clear();
            //userMap.insert(dm->query.value(0).toString(), dm->query.value(1).toString());
            //qDebug() << userMap[dm->query.value(0).toString()];
            ++userCount;
            bigID = dm->query.value(0).toInt();
        }

        //userCount = userMap.size();

        //qDebug() << userCount;
        /*
        if(userCount != -1)
            dirty = 0x11; // 0x11
                       //   ||_____ userDialog;
                       //   |______ groupB0x_user
        */

        return test;
    }

    void sysVal::userHand(int type, int id, QStringList &d)
    {

            switch(type)
            {
                case 0:     // add
                    //dirty = 0x11;
                    ++userCount;
                    bigID = id;
                    qDebug() << "bigID: " << bigID;
                    userMap.insert(id, d);
                    userName << d.at(0);
                    break;
                case 1:     // delete
                    //dirty = 0x11;
                    userName.removeAt(userName.indexOf(userMap[id].at(0)));
                    userMap.remove(id);
                    userCount--;
                    //qDebug() << userCount;
                    break;
                case 2:     // update
                    userMap.insert(id,d);
                    break;
                default:
                    break;
            }
    }

    void sysVal::getSettings()
    {
        // 功能设置
        cfg[0] = sets->value("废品功能", "关").toString();
        cfg[1] = sets->value("重量越限停机", "关").toString();
        cfg[2] = sets->value("重量控制模式", "手动").toString();
        cfg[3] = sets->value("紧头控制模式", "手动").toString();
        cfg[4] = sets->value("香烟牌号", "").toString();
        cfg[5] = sets->value("牌子名称", "").toString();


        // 参数设定
        QString tmp;
        for(int i = 0; i < 32; i++)
        {
            cfg[10+i] = sets->value(QString("%1").arg(i+10), "").toString();
        }

        //
    }

void sysVal::saveSettings()
{
    // 功能设置
    sets->setValue("废品功能", cfg[0]);
    sets->setValue("重量越限停机", cfg[1]);
    sets->setValue("重量控制模式", cfg[2]);
    sets->setValue("紧头控制模式", cfg[3]);
    sets->setValue("香烟牌号", cfg[4]);
    sets->setValue("牌子名称", cfg[5]);


    // 参数设定
    for(int i  = 0; i < 32; i++)
        sets->setValue(QString("%1").arg(10+i), cfg[10+i]);
}

bool sysVal::getBC(int w, QStringList &list)
{
    QString tmp = QString("select t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15 from BANCI where week='%1';")
                        .arg(week.at(w));
    bool test = dm->query.exec(tmp);
    dm->query.next();

    list.clear();
    if(test)
    {
        list << dm->query.value(0).toString()
             << dm->query.value(1).toString()
             << dm->query.value(2).toString()
             << dm->query.value(3).toString()
             << dm->query.value(4).toString()
             << dm->query.value(5).toString()
             << dm->query.value(6).toString()
             << dm->query.value(7).toString()
             << dm->query.value(8).toString()
             << dm->query.value(9).toString()
             << dm->query.value(10).toString()
             << dm->query.value(11).toString()
             << dm->query.value(12).toString()
             << dm->query.value(13).toString()
             << dm->query.value(14).toString()
             << dm->query.value(15).toString();
    }

    //qDebug() << test << list;
    return test;
}

bool sysVal::saveBC(int w, QStringList &list)
{
    QString tmp = QString("update BANCI set t0='%1',").arg(list.at(0));
    tmp.append(QString("t1='%1',").arg(list.at(1)));
    tmp.append(QString("t2='%1',").arg(list.at(2)));
    tmp.append(QString("t3='%1',").arg(list.at(3)));
    tmp.append(QString("t4='%1',").arg(list.at(4)));
    tmp.append(QString("t5='%1',").arg(list.at(5)));
    tmp.append(QString("t6='%1',").arg(list.at(6)));
    tmp.append(QString("t7='%1',").arg(list.at(7)));
    tmp.append(QString("t8='%1',").arg(list.at(8)));
    tmp.append(QString("t9='%1',").arg(list.at(9)));
    tmp.append(QString("t10='%1',").arg(list.at(10)));
    tmp.append(QString("t11='%1',").arg(list.at(11)));
    tmp.append(QString("t12='%1',").arg(list.at(12)));
    tmp.append(QString("t13='%1',").arg(list.at(13)));
    tmp.append(QString("t14='%1',").arg(list.at(14)));
    tmp.append(QString("t15='%1' ").arg(list.at(15)));
    tmp.append(QString("where week='%1';").arg(week.at(w)));

    //qDebug() << tmp;
    bool test = dm->query.exec(tmp);

    return test;
}

void sysVal::updateMap()
{
    for(int j = 6; j > 0; j--)
    {
        QMapIterator<int, int> i(map[j-1]);
        map[j].clear();
        while (i.hasNext())
        {
            i.next();
            map[j][i.key()] = i.value();
        }
    }

    QMapIterator<int, int> i(map[7]);
    map[0].clear();
    while (i.hasNext())
    {
        i.next();
        map[0][i.key()] = i.value();

        //qDebug() << "map[0]" <<  i.key() << i.value();
    }

    map[7].clear();
    map[7][0] = status;
}


QString sysVal::getCS(int id, int n)
{
    QString tmp = QString("select cs%1 from user where id=%2;")
                        .arg(n).arg(id);
    bool test = dm->query.exec(tmp);
    dm->query.next();

    tmp = dm->query.value(0).toString();

    //qDebug() << "getCS: " << test << tmp;
    return tmp;
}

bool sysVal::saveCS(int id, int n, QString &s)
{
    QString tmp = QString("update user set cs%1='%2' where id=%3;")
                        .arg(n).arg(s).arg(id);

    bool test = dm->query.exec(tmp);
    //qDebug() << "saveCS: " << test << tmp;

    return test;
}
