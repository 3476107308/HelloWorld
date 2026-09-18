#include "mywidget.h"
#include "ui_mywidget.h"

#include <QDebug>
#include <QDateTime>

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    //使用SIGNAL/SLOT的方式连接信号与槽
    connect(ui->btnMax,SIGNAL(clicked()),this,SLOT(showMaximized()));

    //使用函数地址的方式连接信号与槽
    connect(ui->btnNormal,&QPushButton::clicked,this,&QWidget::showNormal);

    //a.演示lambda表达式 匿名函数的使用
#if 0
    [](){
        qDebug() << "lambda ";
    }
#endif

#if 0
    [](){
        qDebug() << "lambda ";
    }();
#endif

#if 0
    int sum = [](int x,int y)->int
    {
        return x + y;
    }(1,2);
    qDebug() << sum;
#endif
    //使用lambda表达式做槽函数
    connect(ui->btnTitle,&QPushButton::clicked,this,[this](){
        QString title = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        this->setWindowTitle(title);
    });

}


MyWidget::~MyWidget()
{
    delete ui;
}

//使用设计界面直接点击转到槽函数
void MyWidget::on_btnMin_clicked()
{
    this->showMinimized();
}

