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

# 获取Qt插件生成的所有依赖目标
get_target_property(_plugin_deps tianditu-plugin INTERFACE_LINK_LIBRARIES)

# 收集需要安装的所有目标
set(_targets_to_install tianditu-plugin)
foreach(_dep ${_plugin_deps})
    if(TARGET ${_dep})
        get_target_property(_dep_type ${_dep} TYPE)
        # 只包含内部生成的库目标（排除Qt系统库）
        if(_dep_type MATCHES "STATIC_LIBRARY|OBJECT_LIBRARY" AND _dep MATCHES "^tianditu-plugin")
            list(APPEND _targets_to_install ${_dep})
        endif()
    endif()
endforeach()

# 安装所有相关目标
install(TARGETS ${_targets_to_install}
    EXPORT TiandituPluginTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tianditu-plugin
)

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
