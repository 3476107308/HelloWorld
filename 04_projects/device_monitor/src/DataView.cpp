#include "DataView.h"
#include <QLabel>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>

DataView::DataView(QWidget* parent):QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);

    auto* top_row = new QHBoxLayout();
    counter_label_ = new QLabel(QStringLiteral("rx:0 B"),this);
    hex_check_ = new QCheckBox(QStringLiteral("HEX 显示"),this);
    top_row->addWidget(counter_label_);
    top_row->addStretch();
    top_row->addWidget(hex_check_);

    view_ = new QPlainTextEdit(this);
    view_->setReadOnly(true);
    view_->setMaximumBlockCount(kMaxLines);



    input_ = new QLineEdit(this);
    crlf_check_ = new QCheckBox("追加\\r\\n",this);
    crlf_check_->setChecked(true);
    send_button_ = new QPushButton(QStringLiteral("发送"),this);

    auto* send_row = new QHBoxLayout();
    send_row->addWidget(input_);
    send_row->addWidget(crlf_check_);
    send_row->addWidget(send_button_);

    layout->addLayout(top_row);
    layout->addWidget(view_);
    layout->addLayout(send_row);

    connect(hex_check_,&QCheckBox::toggled,this,[this](bool checked){
        setMode(checked ? DisplayMode::Hex : DisplayMode::Text);
    });
    connect(send_button_,&QPushButton::clicked,this,[this]{
        QByteArray bytes = input_->text().toUtf8();
        if(bytes.isEmpty()) return;
        if(crlf_check_->isChecked()) bytes += "\r\n";
        tx_bytes_ += bytes.size();
        updateCounter();
        emit sendRequested(bytes);
    });

}

void DataView::appendData(const QByteArray& data)
{
    rx_bytes_ += data.size();
    QString text;
    if(mode_ == DisplayMode::Text)
    {
        text = QString::fromUtf8(data);
    }
    else
    {
        text = QString::fromLatin1(data.toHex(' ').toUpper());
    }
    view_->appendPlainText(text);
    updateCounter();
}

void DataView::setMode(DisplayMode mode)
{
    if(mode_ == mode) return;
    if(mode == DisplayMode::Text)
    {
        view_->appendPlainText(QStringLiteral("已经切换到Text模式"));
    }
    if(mode == DisplayMode::Hex)
    {
        view_->appendPlainText(QStringLiteral("已经切换到Hex模式"));
    }
    mode_ = mode;
}

void DataView::updateCounter()
{
    counter_label_->setText(QStringLiteral("rx:%1 B tx:%2 B").arg(rx_bytes_).arg(tx_bytes_));
}