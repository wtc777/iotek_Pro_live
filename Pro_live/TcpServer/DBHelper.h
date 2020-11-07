#ifndef DBHELPER_H
#define DBHELPER_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

class DBHelper
{
public:
    DBHelper();
    static DBHelper& getInstance();
    bool createConnection();
    void removeConnection();
private:
    QSqlDatabase db;

    DBHelper(const DBHelper&) = delete ;
    const DBHelper& operator =(const DBHelper& rhs) = delete ;
};

#endif // DBHELPER_H
