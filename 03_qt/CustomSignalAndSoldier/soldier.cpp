#include "soldier.h"

#include <QDebug>

Soldier::Soldier(QObject *parent)
    : QObject{parent}
{}

void Soldier::fight()
{
    qDebug() << "fight" ;
}

void Soldier::fight(QString s)
{
    //qDebug() << "fight for " << s;
    qDebug().noquote() << "fight for " << s;
    //qDebug().noquote() 用于禁用字符串输出的自动引号包裹，让输出更干净，可读
}
