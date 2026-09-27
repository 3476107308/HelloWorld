#ifndef CONNECTBAR_H
#define CONNECTBAR_H
#include <QWidget>
#include "ConnectionConfig.h"

class QPushButton;
class QLineEdit;
class QLabel;

class ConnectBar: public QWidget
{
    Q_OBJECT
public:
    explicit ConnectBar(QWidget* parent = nullptr);
    enum class Status{
        Disconnected,Connecting,Connected,Error
    };
    void setStatus(Status s,const QString& text = QString());
signals:
    void openRequested(const ConnectionConfig& cfg);
    void closeRequested();

private:
    void onOpenClicked();

    QLineEdit* host_input_ = nullptr;
    QLineEdit* port_input_ = nullptr;
    QPushButton* open_button_ = nullptr;
    QLabel* status_label_ = nullptr;

    Status status_ = Status::Disconnected;
};
#endif // CONNECTBAR_H
