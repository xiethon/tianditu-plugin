project(
    TianDiTuPlugin-example
    VERSION 1.0.0
    LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/xiethon/tianditu-plugin"
    DESCRIPTION "An example application for TianDiTu Qt Location Service Provider Plugin"
)

find_package(Qt6 REQUIRED COMPONENTS Quick Positioning Location Network Sql)

set(CMAKE_AUTOMOC ON)

qt_add_executable(location-example 
    example.cc
)

qt_add_qml_module(location-example
    URI LocationExample
    VERSION 1.0
    QML_FILES Example.qml 
)

target_link_libraries(location-example  
    PRIVATE Qt6::Quick 
    PRIVATE Qt6::Qml
    PRIVATE Qt6::Network
    PRIVATE Qt6::Location
    PRIVATE Qt6::Positioning
    PRIVATE Qt6::Sql

    PRIVATE TianDiTuPlugin
)