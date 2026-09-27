#include "ConnectBar.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

ConnectBar::ConnectBar(QWidget* parent):QWidget(parent)
{
    host_input_ = new QLineEdit(this);
    host_input_->setText(QStringLiteral("127.0.0.1"));
    port_input_ = new QLineEdit(this);
    port_input_->setText(QStringLiteral("8888"));

    open_button_ = new QPushButton(this);
    status_label_ = new QLabel(this);

    auto* layout = new QHBoxLayout(this);
    layout->addWidget(host_input_);
    layout->addWidget(port_input_);
    layout->addWidget(open_button_);
    layout->addStretch();
    layout->addWidget(status_label_);

    connect(open_button_,&QPushButton::clicked,this,&ConnectBar::onOpenClicked);

    setStatus(Status::Disconnected);
}

void ConnectBar::setStatus(Status s,const QString& text)
{
    status_ = s;
    switch(s)
    {
    case Status::Disconnected:
        status_label_->setText(text.isEmpty() ? QStringLiteral("未连接") : text);
        status_label_->setStyleSheet(QStringLiteral("color:gray;"));
        open_button_->setText(QStringLiteral("打开"));
        open_button_->setEnabled(true);
        break;

    case Status::Connecting:
        status_label_->setText(QStringLiteral("正在连接..."));
        status_label_->setStyleSheet(QStringLiteral("color:orange;"));
        open_button_->setEnabled(false);
        break;

    case Status::Connected:
        status_label_->setText(QStringLiteral("已连接"));
        status_label_->setStyleSheet(QStringLiteral("color:green;"));
        open_button_->setText(QStringLiteral("关闭"));
        open_button_->setEnabled(true);
        break;

    case Status::Error:
        status_label_->setText(text.isEmpty() ? QStringLiteral("连接失败") : text);
        status_label_->setStyleSheet(QStringLiteral("color:red;"));
        open_button_->setText(QStringLiteral("打开"));
        open_button_->setEnabled(true);
        break;
    }
}

void ConnectBar::onOpenClicked()
{
    if(status_ == Status::Connected)
    {
        emit closeRequested();
        return;
    }

    if(host_input_->text().isEmpty())
    {
        setStatus(Status::Error,QStringLiteral("客户端没有输入"));
        return;
    }

    if(port_input_->text().isEmpty())
    {
        setStatus(Status::Error,QStringLiteral("端口没有输入"));
        return;
    }

    bool ok = false;
    const int port = port_input_->text().toInt(&ok);
    if(!ok)
    {
        setStatus(Status::Error,QStringLiteral("端口必须是数字"));
        return;
    }

    if(port < 1 || port > 65535)
    {
        setStatus(Status::Error,QStringLiteral("端口范围是1-65535"));
        return;
    }

    ConnectionConfig cfg;
    cfg.host = host_input_->text();
    cfg.port = static_cast<quint16>(port);

    setStatus(Status::Connecting);
    emit openRequested(cfg);

}