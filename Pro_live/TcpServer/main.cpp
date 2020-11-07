#include "MainWindow.h"

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
    MainWindow w;
    w.show();
    splash.finish(&w);

    return a.exec();
}
