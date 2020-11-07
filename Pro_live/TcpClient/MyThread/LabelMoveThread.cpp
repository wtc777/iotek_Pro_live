#include "LabelMoveThread.h"

LabelMoveThread::LabelMoveThread(QLabel *lb, const QPoint &Point, QObject *parent, unsigned long flushTime)
    : QThread(parent),
      lb(lb),
      Point(Point),
      flushTime(flushTime)
{

}

void LabelMoveThread::run()
{
    int x = Point.rx();
    for(; x+this->lb->rect().width() > 0; x-=10){
        Point.setX(x);
        emit moveLabel(Point,this->lb);
        usleep(flushTime);
    }
    emit moveComplete(this->lb);
//    lb->hide();
}
