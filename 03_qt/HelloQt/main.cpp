#include "mywidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //创建Qt应用程序对象，管理事件循环和全局资源
    QApplication a(argc, argv);

    //创建主窗口对象，并调用其show方法，将窗口显示出来
    MyWidget w;
    w.show();

    //调用QCoreApplication类的exec方法
    //程序并不会退出，而是进入事件循环，等待用户交互，直到窗口关闭（比图点击了窗口右上角的关闭按钮）
    return QCoreApplication::exec();
}
