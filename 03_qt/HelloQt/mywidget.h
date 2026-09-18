#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
//在此声明一个MyWidget类，这个类定义在Ui命名空间中
//因为下面会定义一个 UI::MyWidget类型的指针 *ui
namespace Ui
{
    class MyWidget;
}
QT_END_NAMESPACE

//自定义的MyWidget类，要继承自Qt框架提供的QMainWindow/QDialog/QWidget这三个类其中之一，才可以正常显示
class MyWidget : public QWidget
{
    // 启用Qt 的信号/槽机制 （MOC元对象系统必须的）
    Q_OBJECT

public:
    //声明构造函数、析构函数
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget() override;

private:
    //定义一个Ui：：MyWidget类型的指针*ui
    //Ui：：MyWidget这个类定义在ui_mywidget.h中
    //这个Ui：：MyWidget类，本身是空实现，但是它继承自Ui_MyWidget类

    //Ui_MyWidget类，是和UI设计界面一一对应的，也就和mywidget.ui这个xml文件一一对应的
    Ui::MyWidget *ui;
};
#endif // MYWIDGET_H
