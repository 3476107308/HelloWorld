cmake_minimum_required(VERSION 3.19)

project(device_monitor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)
qt_standard_project_setup()
qt_add_executable(device_monitor
    src/main.cpp
    src/mainwindow.cpp


    include/mainwindow.h
    include/device.h
    src/DeviceListModel.cpp
    include/DeviceListModel.h
    include/TempChartWidget.h
    src/TempChartWidget.cpp
    include/JsonLineParser.h
    src/JsonLineParser.cpp
    include/Transport.h
    include/TcpTransport.h
    src/TcpTransport.cpp
    include/DataView.h
    src/DataView.cpp
    include/ConnectionConfig.h
    include/ConnectBar.h
    src/ConnectBar.cpp
    include/IoWorker.h
    src/IoWorker.cpp



)

target_include_directories(device_monitor PRIVATE include)
target_compile_options(device_monitor PRIVATE -Wall -Wextra)


target_link_libraries(device_monitor
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
)
