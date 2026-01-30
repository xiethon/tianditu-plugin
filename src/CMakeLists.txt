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

target_include_directories(tianditu-plugin 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/tianditu-plugin>
)

# 设置导出名称和别名
add_library(TiandituPlugin::tianditu-plugin ALIAS tianditu-plugin)

# 安装目标 - 只导出主目标，不导出Qt插件内部生成的辅助目标
install(TARGETS tianditu-plugin
    EXPORT TiandituPluginTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tianditu-plugin
)

# 导出Qt插件自动生成的内部目标（如果存在）
if(TARGET tianditu-plugin_init)
    install(TARGETS tianditu-plugin_init
        EXPORT TiandituPluginTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

if(TARGET tianditu-plugin_resources_1)
    install(TARGETS tianditu-plugin_resources_1
        EXPORT TiandituPluginTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

# 安装头文件
install(FILES
    geomapsource.h
    geoserviceproviderfactory.h
    geotiledmap.h
    geotiledmappingmanagerengine.h
    geotiledmapreply.h
    geotilefetcher.h
    sqlitetilecache.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tianditu-plugin
)

# 安装资源文件
install(FILES
    resources_url_config.json
    plugin.json
    DESTINATION ${CMAKE_INSTALL_DATADIR}/tianditu-plugin
)
