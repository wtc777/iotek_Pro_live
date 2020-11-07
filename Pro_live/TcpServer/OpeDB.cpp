#include "OpeDB.h"
#include "DBHelper.h"
#include <QDebug>
OpeDB::OpeDB(QWidget *parent) : QWidget(parent)
{

}
OpeDB *OpeDB::instance = nullptr;
OpeDB *OpeDB::getInstance()
{
    if(nullptr == instance){
        instance = new OpeDB;
    }
    return instance;
}

//INSERT INTO usrInfo (uname,upwd)VALUES('jack','123');
bool OpeDB::handleRegist(const char *name, const char *pwd)
{
    if(nullptr == name || nullptr == pwd){
        return false;
    }
    bool ret = DBHelper::getInstance().createConnection();
    if(ret){
        QSqlQuery query;
        query.prepare("INSERT INTO usrInfo (uname,upwd)VALUES(:name,:pwd);");
        query.bindValue(":name",name);
        query.bindValue(":pwd",pwd);
        ret = query.exec();
        if(!ret){
            qDebug()<<query.lastError().text();
        }
    }
    DBHelper::getInstance().removeConnection();
    return ret;
}

bool OpeDB::handleLogin(const char *name, const char *pwd)
{
    if(nullptr == name || nullptr == pwd){
        return false;
    }
    bool ret = DBHelper::getInstance().createConnection();
    if(ret){
        QSqlQuery query;
        query.prepare("SELECT * FROM usrInfo "
                      "WHERE uname = :name AND upwd = :pwd AND online = 0");
        query.bindValue(":name",name);
        query.bindValue(":pwd",pwd);
        ret = query.exec();
        if(!ret){
            qDebug()<<query.lastError().text();
        }else{
            ret = query.next();
            if(ret){
                query.clear();
                query.prepare("UPDATE usrInfo SET online = 1 WHERE uname = :name");
                query.bindValue(":name",name);
                ret = query.exec();
                if(!ret){
                    qDebug()<<query.lastError().text();
                }
            }
        }
    }
    DBHelper::getInstance().removeConnection();
    return ret;
}

void OpeDB::handleOffline(const char *name)
{
    if(nullptr == name){
        return;
    }
    bool ret = DBHelper::getInstance().createConnection();
    if(ret){
        QSqlQuery query;
        query.prepare("UPDATE usrInfo SET online = 0 "
                      "WHERE uname = :name");
        query.bindValue(":name",name);
        ret = query.exec();
        if(!ret){
            qDebug()<<query.lastError().text();
        }
    }
    DBHelper::getInstance().removeConnection();
}

void OpeDB::handleAlloffline()
{
    bool ret = DBHelper::getInstance().createConnection();
    if(ret){
        QSqlQuery query;
        query.prepare("UPDATE usrInfo SET online = 0 ;");
        ret = query.exec();
        if(!ret){
            qDebug()<<query.lastError().text();
        }
    }
    DBHelper::getInstance().removeConnection();
}

int OpeDB::handleGetScore(const char *name)
{
    if(nullptr == name){
        return 0;
    }
    int score = 0;
    bool ret = DBHelper::getInstance().createConnection();
    if(ret){
        QSqlQuery query;
        query.prepare("SELECT uscore FROM usrInfo "
                      "WHERE uname = :name");
        query.bindValue(":name",name);
        ret = query.exec();
        if(!ret){
            qDebug()<<query.lastError().text();
        }else{
            ret = query.next();
            if(ret){
                score = query.value(0).toInt();
            }
        }
    }
    DBHelper::getInstance().removeConnection();
    return score;
}

void OpeDB::handleupdateScore(const char *name, int score)
{
    bool ret = DBHelper::getInstance().createConnection();
    if(ret){
        QSqlQuery query;
        query.prepare("UPDATE usrInfo SET uscore = :score WHERE uname = :name;");
        query.bindValue(":name",name);
        query.bindValue(":score",score);
        ret = query.exec();
        if(!ret){
            qDebug()<<query.lastError().text();
        }
    }
    DBHelper::getInstance().removeConnection();
}
