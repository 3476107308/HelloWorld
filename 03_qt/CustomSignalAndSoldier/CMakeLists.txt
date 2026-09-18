cmake_minimum_required(VERSION 3.19)
project(CustomSignalAndSoldier LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(CustomSignalAndSoldier
    WIN32 MACOSX_BUNDLE
    main.cpp
    mywidget.cpp
    mywidget.h
    mywidget.ui
    commander.h commander.cpp
    soldier.h soldier.cpp
)

target_link_libraries(CustomSignalAndSoldier
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS CustomSignalAndSoldier
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET CustomSignalAndSoldier
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
