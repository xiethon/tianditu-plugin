set(QT_NO_PRIVATE_MODULE_WARNING ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Positioning Sql Location LocationPrivate Network)

qt_add_plugin(tianditu-plugin
    STATIC
    CLASS_NAME GeoServiceProviderFactory
        geomapsource.cc
        geomapsource.h
        geoserviceproviderfactory.cc
        geoserviceproviderfactory.h
        geotiledmap.cc
        geotiledmap.h
        geotiledmappingmanagerengine.cc
        geotiledmappingmanagerengine.h
        geotiledmapreply.cc 
        geotiledmapreply.h
        geotilefetcher.cc
        geotilefetcher.h
        sqlitetilecache.cc
        sqlitetilecache.h
)

qt_add_resources(tianditu-plugin
    PREFIX "/"
    FILES resources_url_config.json
)

target_link_libraries(tianditu-plugin
    PRIVATE
        Qt6::Positioning
        Qt6::Sql
        Qt6::LocationPrivate
    PUBLIC
        Qt6::Core
        Qt6::Location
        Qt6::Network
)

target_include_directories(tianditu-plugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
