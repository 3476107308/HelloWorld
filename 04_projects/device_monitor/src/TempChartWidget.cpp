#include "TempChartWidget.h"
#include <QPainter>
#include <QPolygon>
#include <QPointF>
#include <QRectF>
#include <QColor>
#include <cmath>


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

    double lo = kMinTemp;
    double hi = kMaxTemp;
    for(double v:points_)
    {
        lo = qMin(lo,v);
        hi = qMax(hi,v);
    }
    lo = std::floor(lo / kStep) * kStep;
    hi = std::ceil(hi / kStep) * kStep;
    const double span = hi -lo;
    auto toY = [&](double v){
        return plot.top() + plot.height() - (v - lo) / span * plot.height();
    };

    for(double v = lo; v <= hi + 1e-6; v += kStep)
    {
        double y = toY(v);

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
        double y = toY(points_[i]);
        poly << QPointF(x,y);
    }

    p.save();
    p.setClipRect(plot);
    p.setPen(QPen(Qt::blue,2));
    p.drawPolyline(poly);
    p.restore();
    }
}