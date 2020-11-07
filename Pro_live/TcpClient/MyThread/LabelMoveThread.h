#ifndef LABELMOVETHREAD_H
#define LABELMOVETHREAD_H

#include <QObject>
#include <QThread>
#include <QLabel>
class LabelMoveThread : public QThread
{
    Q_OBJECT
public:
    explicit LabelMoveThread(QLabel*lb,
                             const QPoint& Point, QObject *parent = nullptr,
                             unsigned long flushTime = 100000);
    QLabel* getLabel(){return this->lb;}
signals:
    void moveLabel(const QPoint& point,QLabel* lb);
    void moveComplete(QLabel* lb);
public slots:

protected:
    void run();
private:
    QLabel * lb;                //弹幕本体控件指针
    QPoint Point;               //弹幕起始点
    unsigned long flushTime;    //弹幕移动周期
};

#endif // LABELMOVETHREAD_H
