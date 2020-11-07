#ifndef OPEDB_H
#define OPEDB_H

#include <QWidget>

class OpeDB : public QWidget
{
    Q_OBJECT
public:    
    static OpeDB *getInstance();

    bool handleRegist(const char* name,const char* pwd);//用户注册函数
    bool handleLogin(const char* name,const char* pwd);//用户登录验证函数
    void handleOffline(const char* name);//客户端离线状态修改函数
    void handleAlloffline();//初始化所有用户为离线状态
    int handleGetScore(const char* name);//查询用户积分余额
    void handleupdateScore(const char* name, int score);//修改用户积分余额
signals:

public slots:
private:
    explicit OpeDB(QWidget *parent = nullptr);
    OpeDB(const OpeDB&) = delete ;
    const OpeDB& operator =(const OpeDB&) = delete ;
    static OpeDB* instance;
};

#endif // OPEDB_H
