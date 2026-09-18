#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QObject>
#include <QByteArray>
#include <QString>

class Transport: public QObject
{
    Q_OBJECT
public:
    Transport() = default;
    Transport(QObject* parent):QObject(parent){}
    virtual ~Transport() = default;
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool isConnected()const = 0;
    virtual void sendByte(const QByteArray& chunk) = 0;

signals:
    void receiveByte(const QByteArray& data);
    void openSucceeded();
    void openError(const QString& reason);
};
#endif // TRANSPORT_H
