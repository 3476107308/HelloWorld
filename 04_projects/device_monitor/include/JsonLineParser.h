#ifndef JSONLINEPARSER_H
#define JSONLINEPARSER_H

#include <QObject>
#include <QJsonObject>
#include <QByteArray>
#include <QString>

class JsonLineParser   : public QObject
{
    Q_OBJECT
public:
    JsonLineParser(QObject* parent);
    void appendData(const QByteArray& chunk);

signals:
    void messageReceived(const QJsonObject& obj);
    void parseFailed(const QString& reason);

private:
    QByteArray buffer_;
    void parseLine(const QByteArray&);
};
#endif // JSONLINEPARSER_H
