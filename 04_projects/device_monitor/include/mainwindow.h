#ifndef DEVICE_MONITOR_MAINWINDOW_H
#define DEVICE_MONITOR_MAINWINDOW_H

#include <QMainWindow>
#include "device.h"
#include <QVector>


class QPushButton;
class QLabel;
class QListView;
class QTextEdit;
class QTimer;
class DeviceListModel;
class TempChartWidget;
class MessageParser;
class QByteArray;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private:
    void setupUi();
    void refreshDeviceData();
    void saveConfig();
    void loadConfig();
    void writeLog(const QString& msg);

    QListView* device_list_ = nullptr;
    QLabel* selected_device_label_ = nullptr;
    QLabel* status_label_ = nullptr;
    QLabel* telemetry_label_ = nullptr;
    QTextEdit* log_view_ = nullptr;
    QVector<Device> devices_;
    QTimer* refresh_timer_ = nullptr;
    QPushButton* save_button_ = nullptr;
    QPushButton* load_button_ = nullptr;
    DeviceListModel* device_model_ = nullptr;
    TempChartWidget* temp_chart_ = nullptr;
    MessageParser* parser_ = nullptr;
    QTimer* sim_timer = nullptr;
    QByteArray pending_;

public slots:
    void updateDeviceDetails(int row);
    void simulateCommData();
};

#endif  // DEVICE_MONITOR_MAINWINDOW_H
