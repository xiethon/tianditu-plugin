cmake_minimum_required(VERSION 3.22.1)

project(
    tianditu-plugin
    VERSION 1.0.0
    LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/xiethon/tianditu-plugin"
    DESCRIPTION "A Qt Location Service Provider Plugin for TianDitu Maps"
)

# 使用GNUInstallDirs提供标准安装路径
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(BUILD_EXAMPLE "Build example" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif(BUILD_EXAMPLE)

# 生成版本文件
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/TiandituPluginConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# 导出目标
install(EXPORT TiandituPluginTargets
    FILE TiandituPluginTargets.cmake
    NAMESPACE TiandituPlugin::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TiandituPlugin
)

# 配置并安装Config文件
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TiandituPluginConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/TiandituPluginConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TiandituPlugin
)

# 安装CMake配置文件
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/TiandituPluginConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/TiandituPluginConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TiandituPlugin
)

