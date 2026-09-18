#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include "commander.h"
#include "soldier.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MyWidget;
}
QT_END_NAMESPACE

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget() override;

private:
    Ui::MyWidget *ui;
    Commander* commander_;
    soldier* soldier_;
};
#endif // MYWIDGET_H
