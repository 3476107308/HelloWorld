#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>

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

signals:
    void loginSuccess(const QString& username);

public slots:
    void onBtnLoginClicked();
};
#endif // MYWIDGET_H
