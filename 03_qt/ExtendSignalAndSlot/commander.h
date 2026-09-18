#ifndef COMMANDER_H
#define COMMANDER_H

#include <QWidget>

class Commander : public QWidget
{
    Q_OBJECT
public:
    explicit Commander(QWidget *parent = nullptr);

signals:
    void go();
};

#endif // COMMANDER_H
