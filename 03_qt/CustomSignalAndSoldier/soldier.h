#ifndef SOLDIER_H
#define SOLDIER_H

#include <QObject>

class Soldier : public QObject
{
    Q_OBJECT
public:
    explicit Soldier(QObject *parent = nullptr);

signals:

public slots:
    //public表示槽函数既可以在当前类以及其子类的成员函数中使用，也可以在类外部其他函数中使用
    void fight();
    void fight(QString s);

};

#endif // SOLDIER_H
