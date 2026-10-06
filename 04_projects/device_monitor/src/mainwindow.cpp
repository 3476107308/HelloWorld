#include "mainwindow.h"

constexpr double kMaxTemperature = 45.0;
constexpr double kMaxVoltage = 4.0;

#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QSplitter>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QString>
#include <QTimer>
#include <QRandomGenerator>
#include <QPushButton>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDateTime>
#include <QListView>
#include <QItemSelectionModel>
#include <QThread>

#include "TempChartWidget.h"
#include "DeviceListModel.h"
#include "JsonLineParser.h"
#include "IoWorker.h"
#include "DataView.h"
#include "ConnectBar.h"



MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("Device Monitor"));
    resize(900, 600);
    qDebug() << "[MainWindow] UI thread =" << QThread::currentThread();
    setupUi();
}

MainWindow::~MainWindow()
{
    io_thread_->quit();
    io_thread_->wait();
    delete worker_;
}

void MainWindow::setupUi()
{
    auto* central_widget = new QWidget(this);
    auto* root_layout = new QVBoxLayout(central_widget);
    auto* content_splitter = new QSplitter(Qt::Horizontal, central_widget);


    auto* device_panel = new QGroupBox(QStringLiteral("设备列表"), content_splitter);
    auto* device_layout = new QVBoxLayout(device_panel);
    device_list_ = new QListView(device_panel);
    device_layout->addWidget(device_list_);

    auto* detail_panel = new QGroupBox(QStringLiteral("设备详情"), content_splitter);
    auto* detail_layout = new QFormLayout(detail_panel);
    selected_device_label_ = new QLabel(QStringLiteral("未选择"), detail_panel);
    status_label_ = new QLabel(QStringLiteral("离线"), detail_panel);
    telemetry_label_ = new QLabel(QStringLiteral("温度: --\n电压: --"), detail_panel);
    detail_layout->addRow(QStringLiteral("设备"), selected_device_label_);
    detail_layout->addRow(QStringLiteral("状态"), status_label_);
    detail_layout->addRow(QStringLiteral("数据"), telemetry_label_);


    setCentralWidget(central_widget);

    content_splitter->addWidget(device_panel);
    content_splitter->addWidget(detail_panel);
    content_splitter->setStretchFactor(0, 1);
    content_splitter->setStretchFactor(1, 2);

    auto* log_panel = new QGroupBox(QStringLiteral("运行日志"), central_widget);
    auto* log_layout = new QVBoxLayout(log_panel);
    log_view_ = new QTextEdit(log_panel);
    log_view_->setReadOnly(true);
    log_view_->setPlainText(QStringLiteral("程序已启动，等待设备数据。"));
    log_layout->addWidget(log_view_);



    // TODO: 阶段 1 再把设备数据、QTimer 和选择变化连接到这些显示控件。  已经完成
    devices_.append({QStringLiteral("1"),QStringLiteral("device01"),true,0.0,1.0});
    devices_.append({QStringLiteral("2"),QStringLiteral("device02"),false,8.2,5.0});
    devices_.append({QStringLiteral("3"),QStringLiteral("device03"),false,0.3,6.0});

    device_model_ = new DeviceListModel(devices_,this);
    device_list_->setModel(device_model_);
    connect(device_list_->selectionModel(),&QItemSelectionModel::currentChanged,this,[this](const QModelIndex& current,const QModelIndex&){
        updateDeviceDetails(current.row());
    });


    refresh_timer_ = new QTimer(this);
    connect(refresh_timer_,&QTimer::timeout,this,&MainWindow::refreshDeviceData);
    //refresh_timer_->start(1000);

    save_button_ = new QPushButton(QStringLiteral("保存配置"),detail_panel);
    detail_layout->addRow(QStringLiteral("操作"),save_button_);
    connect(save_button_,&QPushButton::clicked,this,&MainWindow::saveConfig);

    load_button_ = new QPushButton(QStringLiteral("下载配置"),detail_panel);
    detail_layout->addRow(QStringLiteral("下载"),load_button_);
    connect(load_button_,&QPushButton::clicked,this,&MainWindow::loadConfig);

    auto* chart_panel = new QGroupBox(QStringLiteral("温度曲线"),central_widget);
    auto* chart_layout = new QVBoxLayout(chart_panel);
    temp_chart_ = new TempChartWidget(chart_panel);
    temp_chart_->setMinimumHeight(120);
    chart_layout->addWidget(temp_chart_);

    connect_bar_ = new ConnectBar(central_widget);
    root_layout->addWidget(connect_bar_);
    connect(connect_bar_,&ConnectBar::openRequested,this,[this](const ConnectionConfig& cfg){
        emit requestOpen(cfg);
    });
    connect(connect_bar_,&ConnectBar::closeRequested,this,[this]{
        emit requestClose();
    });
    root_layout->addWidget(content_splitter,1);
    root_layout->addWidget(chart_panel);
    auto* data_panel = new QGroupBox(QStringLiteral("收发框"),central_widget);
    auto* data_panel_layout = new QVBoxLayout(data_panel);
    data_view_ = new DataView(data_panel);
    connect(data_view_,&DataView::sendRequested,this,[this](const QByteArray& data){
        emit requestSend(data);
    });
    data_panel_layout->addWidget(data_view_);
    root_layout->addWidget(data_panel);

    root_layout->addWidget(log_panel);

    parser_ = new JsonLineParser(this);
    connect(parser_,&JsonLineParser::messageReceived,this,[this](const QJsonObject& obj){
        writeLog(QStringLiteral("收到消息：id=%1 温度%2 电压%3")
                     .arg(obj["id"].toString())
                     .arg(obj["temperature"].toDouble())
                     .arg(obj["voltage"].toDouble()));
        temp_chart_->addValue(obj["temperature"].toDouble());
    });

    connect(parser_,&JsonLineParser::parseFailed,this,[this](const QString& reason){
        writeLog(QStringLiteral("解析失败:") + reason);
    });


    io_thread_ = new QThread(this);
    worker_ = new IoWorker();
    worker_->moveToThread(io_thread_);



    connect(worker_,&IoWorker::connectionOpened,this,[this]{
        writeLog(QStringLiteral("连接成功"));
        connect_bar_->setStatus(ConnectBar::Status::Connected);
    });
    connect(this,&MainWindow::requestOpen,worker_,&IoWorker::openConnection);

    connect(worker_,&IoWorker::connectionFailed,this,[this](const QString& reason){
        writeLog(QStringLiteral("连接失败:") + reason);
        connect_bar_->setStatus(ConnectBar::Status::Error,reason);
    });

    connect(worker_,&IoWorker::connectionClosed,this,[this]{
        writeLog(QStringLiteral("断开连接"));
        connect_bar_->setStatus(ConnectBar::Status::Disconnected);
    });
    connect(this,&MainWindow::requestClose,worker_,&IoWorker::closeConnection);

    connect(worker_,&IoWorker::sendFailed,this,[this](const QString& reason){
        writeLog(QStringLiteral("发送失败:") + reason);
    });
    connect(this,&MainWindow::requestSend,worker_,&IoWorker::sendData);

    connect(worker_,&IoWorker::dataReceived,this,[this](const QByteArray& data){
        writeLog(QStringLiteral("接收成功，数据为:") + QString::fromUtf8(data));
        data_view_->appendData(data);
        parser_->appendData(data);
    });


    connect(io_thread_,&QThread::started,worker_,&IoWorker::start);
    io_thread_->start();

    auto* display_splitter = new QSplitter(Qt::Vertical);
    display_splitter->addWidget(chart_panel);
    display_splitter->addWidget(log_panel);

    auto* io_splitter = new QSplitter(Qt::Horizontal);
    io_splitter->addWidget(data_panel);
    io_splitter->addWidget(display_splitter);

    auto* main_splitter = new QSplitter(Qt::Vertical);
    main_splitter->addWidget(io_splitter);
    main_splitter->addWidget(content_splitter);

    main_splitter->setSizes({500,120});
    display_splitter->setSizes({250,200});
    io_splitter->setSizes({400,600});


    main_splitter->setCollapsible(0,false);
    main_splitter->setCollapsible(1,false);
    display_splitter->setCollapsible(0,false);
    display_splitter->setCollapsible(1,false);
    io_splitter->setCollapsible(0,false);
    io_splitter->setCollapsible(1,false);

    main_splitter->setStretchFactor(0,1);
    main_splitter->setStretchFactor(1,0);

    root_layout->addWidget(main_splitter);
}

void MainWindow::updateDeviceDetails(int row)
{
    if(row >= devices_.size() || row < 0) return;
    const Device& device = devices_[row];
    selected_device_label_->setText(device.name_);
    telemetry_label_->setText(QStringLiteral("温度：%1\n电压：%2")
                                  .arg(device.temperature_)
                                  .arg(device.voltage_));

    status_label_->setText(
        device.online_ ? QStringLiteral("在线")
                       : QStringLiteral("离线")
        );
}

void MainWindow::refreshDeviceData()
{
    int row = device_list_->selectionModel()->currentIndex().row();
    if(row < 0 || row >= devices_.size()) return;

    Device& device = devices_[row];
    devices_[row].temperature_ = QRandomGenerator::global()->bounded(20,61);
    devices_[row].voltage_ = QRandomGenerator::global()->bounded(1,6);

    device_model_->deviceDataChanged(row);
    updateDeviceDetails(row);
    bool a = (devices_[row].temperature_ > kMaxTemperature);
    bool b = (devices_[row].voltage_ > kMaxVoltage);
    if(a || b)
    {
        if(!device.alarm_){
            writeLog(QStringLiteral("告警:设备%1 温度%2 电压%3")
                                  .arg(device.name_)
                                  .arg(device.temperature_)
                                  .arg(device.voltage_));
            device.alarm_ = true;
        }
    }
    else
    {
        if(device.alarm_)
        {
            writeLog(QStringLiteral("设备%1恢复正常").arg(device.name_));
            device.alarm_ = false;
        }
    }
    device_model_->deviceDataChanged(row);
    updateDeviceDetails(row);
}

void MainWindow::saveConfig()
{
    QJsonArray deviceArray;
    for(const auto& device: devices_)
    {
        QJsonObject obj;
        obj["id"]          = device.id_;
        obj["name"]        = device.name_;
        obj["online"]      = device.online_;
        obj["temperature"] = device.temperature_;
        obj["voltage"]     = device.voltage_;
        obj["alarm"]       = device.alarm_;
        deviceArray.append(obj);
    }
    QJsonDocument doc(deviceArray);
    QByteArray data = doc.toJson(QJsonDocument::Indented);

    QFile file(QStringLiteral("config.json"));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        writeLog(QStringLiteral("打开配置文件失败"));
        return;
    }
    file.write(data);
    file.close();
    writeLog(QStringLiteral("配置已经保存"));
}

void MainWindow::loadConfig()
{
    QFile file(QStringLiteral("config.json"));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        writeLog(QStringLiteral("打开配置文件失败"));
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data,&err);
    if(doc.isNull() || err.error != QJsonParseError::NoError)
    {
        writeLog(QStringLiteral("解析配置失败：") + err.errorString());
        return;
    }

    QJsonArray arr = doc.array();
    devices_.clear();
    for(const QJsonValue& value: arr)
    {
        QJsonObject obj = value.toObject();
        Device device;
        device.id_          = obj["id"].toString();
        device.name_        = obj["name"].toString();
        device.online_      = obj["online"].toBool();
        device.temperature_ = obj["temperature"].toDouble();
        device.voltage_     = obj["voltage"].toDouble();
        device.alarm_       = obj["alarm"].toBool();
        devices_.append(device);
    }

    device_model_->reload();                                    // 数据变了，让模型通知视图重新取数
    if(!devices_.isEmpty())
        device_list_->selectionModel()->setCurrentIndex(
            device_model_->index(0,0), QItemSelectionModel::Select);   // 选中第一行 → 刷新详情
}

void MainWindow::writeLog(const QString& msg)
{
    log_view_->append(msg);
    QFile file(QStringLiteral("run.log"));
    if(file.open(QIODevice::Append | QIODevice::Text))
    {
        QString line = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QStringLiteral(" ") + msg;
        file.write(line.toUtf8() + "\n");
        file.close();
    }
}
