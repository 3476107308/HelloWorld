#include "mywidget.h"
#include "ui_mywidget.h"

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    //1.创建2个类的实例
    commander_ = new Commander(this);
    soldier_ = new Soldier(this);
    //2.建立信号和槽的连接

    //会产生二义性，编译器会报错
    //connect(commander_,&Commander::go,soldier_,&Soldier::fight);
    //使用QOverload 重载信号槽
    connect(commander_,QOverload<>::of(&Commander::go),soldier_,QOverload<>::of(&Soldier::fight));
    connect(commander_,QOverload<QString>::of(&Commander::go),soldier_,QOverload<QString>::of(&Soldier::fight));

    //3.发送信号
    emit commander_->go();
    emit commander_->go("freedom");
}

MyWidget::~MyWidget()
{
    delete ui;
}
