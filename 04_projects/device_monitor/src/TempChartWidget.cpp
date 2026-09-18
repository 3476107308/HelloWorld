#include "TempChartWidget.h"
#include <QPainter>
#include <QPolygon>
#include <QPointF>
#include <QRectF>
#include <QColor>


TempChartWidget::TempChartWidget(QWidget* parent):QWidget(parent){}

void TempChartWidget::addValue(double v)
{
    points_.append(v);
    while(points_.size() > kMaxPoints)
    {
        points_.removeFirst();
    }
    update();
}

void TempChartWidget::clear()
{
    points_.clear();
    update();
}

void TempChartWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);


    constexpr double kMinTemp = 20.0;
    constexpr double kMaxTemp = 60.0;
    constexpr double kStep = 10.0;
    const int leftMargin = 40,bottomMargin = 20,topMargin = 5,rightMargin = 5;

    QPainter p(this);
    p.fillRect(rect(),Qt::white);

    QRectF plot = QRectF(leftMargin,topMargin,width() - leftMargin - rightMargin,height() - topMargin - bottomMargin);
    if(plot.width() <= 0 || plot.height() <= 0) return;

    for(double v = kMinTemp; v <= kMaxTemp + 1e-6; v += kStep)
    {
        double y = plot.top() + plot.height()
        - (v - kMinTemp) / (kMaxTemp - kMinTemp) * plot.height();   // 和曲线同一公式

        p.setPen(QPen(QColor(220, 220, 220), 1));                 // 浅灰细线
        p.drawLine(QPointF(plot.left(), y), QPointF(plot.right(), y));

        p.setPen(Qt::black);                                        // 刻度数字
        p.drawText(QRectF(0, y - 8, leftMargin - 4, 16),
                   Qt::AlignRight | Qt::AlignVCenter,
                   QString::number(int(v)));
    }

    // ② 两条轴
    p.setPen(Qt::black);
    p.drawLine(QPointF(plot.left(), plot.top()),    QPointF(plot.left(), plot.bottom()));   // Y 轴(左竖)
    p.drawLine(QPointF(plot.left(), plot.bottom()), QPointF(plot.right(), plot.bottom()));  // X 轴(底横)

    if(!points_.isEmpty())
    {
    QPolygonF poly;
    for(int i = 0;i < points_.size();i++)
    {
        double x = plot.left() + plot.width() * double(i) / (kMaxPoints - 1);
        double y = plot.top() + plot.height() - (points_[i] - kMinTemp) * plot.height() / (kMaxTemp - kMinTemp);
        poly << QPointF(x,y);
    }


    p.setPen(QPen(Qt::blue,2));
    p.drawPolyline(poly);
    }
}