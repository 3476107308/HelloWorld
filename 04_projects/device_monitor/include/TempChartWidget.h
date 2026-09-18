#ifndef TEMPCHARTWIDGET_H
#define TEMPCHARTWIDGET_H

#include <QWidget>
#include <QVector>

class TempChartWidget: public QWidget
{
    Q_OBJECT
public:
    explicit TempChartWidget(QWidget* parent = nullptr);
    void addValue(double v);
    void clear();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<double> points_;
    static constexpr int kMaxPoints = 60;
};
#endif // TEMPCHARTWIDGET_H
