#ifndef DEVICE_MONITOR_MAINWINDOW_H
#define DEVICE_MONITOR_MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include "device.h"
#include "ConnectionConfig.h"

class QPushButton;
class QLabel;
class QListView;
class QPlainTextEdit;
class QTimer;
class DeviceListModel;
class TempChartWidget;
class JsonLineParser;
class QByteArray;
class IoWorker;
class QThread;
class DataView;
class ConnectBar;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

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
    QPlainTextEdit* log_view_ = nullptr;
    QVector<Device> devices_;
    QTimer* refresh_timer_ = nullptr;
    QPushButton* save_button_ = nullptr;
    QPushButton* load_button_ = nullptr;
    DeviceListModel* device_model_ = nullptr;
    TempChartWidget* temp_chart_ = nullptr;
    JsonLineParser* parser_ = nullptr;
    QThread* io_thread_ = nullptr;
    IoWorker* worker_ = nullptr;
    DataView* data_view_ = nullptr;
    ConnectBar* connect_bar_ = nullptr;

public slots:
    void updateDeviceDetails(int row);

signals:
    void requestOpen(const ConnectionConfig& );
    void requestClose();
    void requestSend(const QByteArray& );
};

#endif  // DEVICE_MONITOR_MAINWINDOW_H
