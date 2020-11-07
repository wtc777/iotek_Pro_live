#include "Log_module/myLog.h"
#include "ui_myLog.h"
#include<QDebug>
#include<QProcess>
#include<QStringList>
myLog::myLog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myLog)
{
    ui->setupUi(this);
    init();
    //LOG("123ABC");
}
myLog* myLog::instance = 0;
myLog *myLog::getInstance()
{
    if(NULL == instance)
    {
        instance = new myLog;
    }
    return instance;
}

void myLog::init()
{
    this->resize(700,600);
    this->setWindowTitle("系统日志信息");

    QString path = QDir::currentPath()+"/ProLog.txt";
    file = new QFile(path,this);
    FileOpenRes = file->open(QIODevice::WriteOnly | QFile::Append);//指定打开方式：只写，追加
    if(!FileOpenRes)
    {
        QMessageBox::warning(this,"日志文件","日志文件打开失败");
    }else
    {
        qDebug()<<"日志文件打开成功";
    }
}

void myLog::LogPrint(const char* whichFile, int whichLine,const QStringList& text)
{
    if(whichFile == nullptr || text.isEmpty())
    {
        qDebug()<<__FILE__<<__LINE__<<"实参错误";
        return ;
    }
    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString LogCodeMsg = QString("%1>>File:%2;Line:%3:")\
                        .arg(date).arg(whichFile).arg(whichLine);
    QStringList logtext(LogCodeMsg);
    logtext.append(text);
//    QString LogText = QString(" >>%1;").arg(text);
//    qDebug()<<LogCodeMsg;
//    qDebug()<<LogText;
    printTOfile(logtext);
//    printTOfile(LogText);
}

void myLog::printTOfile(const QStringList& text)
{
    printToWidget(text);
    QTextStream out(file);
    int i = 0;
    for(; i<text.size(); i++)
    {
        out<<text.at(i)<<'\n';
    }
    //flush(out);
}

void myLog::printToWidget(const QStringList& text)
{
    int i = 0;
    for(; i<text.size(); i++)
    {
        ui->te_Logshow->append(text.at((i)));
    }
}

void myLog::openLogFile()
{
    QProcess process(0);
    QString path = QDir::currentPath()+"/ProLog.txt";
    QStringList list;
    list.clear();
    list.append(path);
    QString cmd = "C:\\Windows\\system32\\notepad.exe";
    process.start(cmd,list,QIODevice::ReadOnly);
    process.waitForStarted();
    process.waitForFinished();
}

myLog::~myLog()
{
    file->close();
//    delete instance;
    delete ui;
}
