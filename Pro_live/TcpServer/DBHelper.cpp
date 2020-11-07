#include "DBHelper.h"
#include <QDebug>

#if 0
MySQL
CREATE TABLE usrInfo(
        uid INTEGER PRIMARY KEY AUTO_INCREMENT,
        uname VARCHAR(32) NOT NULL UNIQUE,
        upwd VARCHAR(32) DEFAULT '123456',
        online INTEGER DEFAULT  0);

sqlite3
CREATE TABLE usrInfo(
        uid INTEGER PRIMARY KEY AUTOINCREMENT,
        uname VARCHAR(32) NOT NULL UNIQUE,
        upwd VARCHAR(32) DEFAULT '123456',
        online INTEGER DEFAULT  0,
        uscore INTEGER DEFAULT 0);
#endif
DBHelper::DBHelper()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
//    db = QSqlDatabase::addDatabase("QMYSQL");
//    db.setHostName("localhost");
//    db.setPort(3306);
//    db.setUserName("root");
//    db.setPassword("123456");
}

DBHelper &DBHelper::getInstance()
{
    static DBHelper instance;
    return instance;
}

bool DBHelper::createConnection()
{
    db.setDatabaseName("usrDB.db");
    if(db.open()){
        qDebug()<<"数据库打开成功";
        return true;
    }else{
        qDebug()<<"数据库打开失败"<<db.lastError().text();
        return false;
    }

}

void DBHelper::removeConnection()
{
    qDebug()<<"数据库已关闭";
    db.removeDatabase("usrDB.db");
    db.close();
}
