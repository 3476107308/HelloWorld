#ifndef DATAVIEW_H
#define DATAVIEW_H

#include <QWidget>
#include <QByteArray>

class QPlainTextEdit;
class QLabel;
class QCheckBox;

class DataView : public QWidget
{
    Q_OBJECT
public:
    enum class DisplayMode{Text,Hex};
    explicit DataView(QWidget* parent = nullptr);

    void appendData(const QByteArray& data);
    void setMode(DisplayMode mode);

private:
    static constexpr int kMaxLines = 500;

    QPlainTextEdit* view_ = nullptr;
    QLabel* counter_label_ = nullptr;
    DisplayMode mode_ = DisplayMode::Text;
    qint64 rx_bytes_ = 0;
    qint64 tx_bytes_ = 0;
    QCheckBox* hex_check_ = nullptr;


};
#endif // DATAVIEW_H
