#include "DataView.h"
#include <QLabel>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QString>
#include <QCheckBox>

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

    layout->addLayout(top_row);
    layout->addWidget(view_);

    connect(hex_check_,&QCheckBox::toggled,this,[this](bool checked){
        setMode(checked ? DisplayMode::Hex : DisplayMode::Text);
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
    counter_label_->setText(QStringLiteral("rx:%1 B").arg(rx_bytes_));
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