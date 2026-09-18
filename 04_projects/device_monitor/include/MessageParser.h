#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <QObject>
#include <QJsonObject>
#include <QByteArray>
#include <QString>

class MessageParser : public QObject
{
    Q_OBJECT
public:
    MessageParser(QObject* parent);
    void appendData(const QByteArray& chunk);

signals:
    void messageReceived(const QJsonObject& obj);
    void parseFailed(const QString& reason);

private:
    QByteArray buffer_;
    void parseLine(const QByteArray&);
};
#endif // MESSAGEPARSER_H
