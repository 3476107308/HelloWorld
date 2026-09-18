#include "mywidget.h"
#include "ui_mywidget.h"

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    //关联信号槽
    connect(ui->btnMax,&QPushButton::clicked,this,&QWidget::showMaximized);
    connect(ui->btnMin,&QPushButton::clicked,this,&QWidget::showMinimized);
    connect(ui->btnNormal,&QPushButton::clicked,this,&QWidget::showNormal);
    connect(ui->btnClose,&QPushButton::clicked,this,&QWidget::close);

}

MyWidget::~MyWidget()
{
    delete ui;
}
