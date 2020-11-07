#include "Login.h"

#include <QApplication>
#include <QSplashScreen>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //程序启动画面
    QPixmap pixmap(":/FBK.png");
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();
    Login* mylogin = Login::getInstance();
    mylogin->show();
    splash.finish(mylogin);
//    Login* mylogin = Login::getInstance();
//    mylogin->show();
    return a.exec();
}
//80.42,22,3389
