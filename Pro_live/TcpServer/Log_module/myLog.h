#ifndef MYLOG_H
#define MYLOG_H

#include <QWidget>
#include <QDateTime>
#include<QDir>
#include<QMessageBox>
#include<QTextStream>

#ifndef _MYLOG_
#define _MYLOG_
#define MYLOG(Msg) myLog::getInstance()->LogPrint(__FILE__,__LINE__,Msg)
#endif

namespace Ui {
class myLog;
}

class myLog : public QWidget
{
    Q_OBJECT

public:
//    explicit myLog(QWidget *parent = 0);
    static myLog* getInstance();
    void init();
    void LogPrint(const char *whichFile, int whichLine, const QStringList &text);
    void printTOfile(const QStringList &text);
    void printToWidget(const QStringList &text);
    void openLogFile();
    ~myLog();

private:
    explicit myLog(QWidget *parent = 0);
    Ui::myLog *ui;
    QFile *file;
    bool FileOpenRes;
    static myLog* instance;
};

#endif // MYLOG_H
