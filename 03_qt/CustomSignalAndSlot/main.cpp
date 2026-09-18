#include "mywidget.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWidget w;
    w.show();

    QObject::connect(&w,&MyWidget::loginSuccess,[](const QString &name){
        QMessageBox::information(nullptr,"登录成功","欢迎, " + name);
    });
    return QCoreApplication::exec();
}
