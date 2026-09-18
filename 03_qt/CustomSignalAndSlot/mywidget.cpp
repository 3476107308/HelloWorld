#include "mywidget.h"
#include "ui_mywidget.h"
#include <QPushButton>
#include <QMessageBox>

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    connect(ui->btnLogin,&QPushButton::clicked,this,&MyWidget::onBtnLoginClicked);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::onBtnLoginClicked()
{
    QString name = ui->username->text();
    if(name.isEmpty()){
        QMessageBox::warning(this,"报错","请输入用户名");
        return;
    }
    emit loginSuccess(name);
}