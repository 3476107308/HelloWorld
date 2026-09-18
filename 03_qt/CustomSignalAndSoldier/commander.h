#ifndef COMMANDER_H
#define COMMANDER_H

#include <QObject>

class Commander : public QObject
{
    Q_OBJECT
public:
    explicit Commander(QObject *parent = nullptr);

signals:
    //信号函数只需要声明，不需要实现
    void go();
    void go(QString s);
};

#endif // COMMANDER_H
