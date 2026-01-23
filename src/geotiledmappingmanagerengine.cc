
//! 地图瓦片管理引擎类，实现地图瓦片管理引擎的核心功能，包括地图源初始化、瓦片获取和缓存管理
#include "geotiledmappingmanagerengine.h"
#include "geotiledmap.h"
#include "geotilefetcher.h"

#include <QtLocation/private/qgeocameracapabilities_p.h>
#include <QtLocation/private/qgeofiletilecache_p.h>
#include <QtLocation/private/qgeomaptype_p.h>
#include <QtLocation/private/qgeotiledmap_p.h>

#include "sqlitetilecache.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

GeoTiledMappingManagerEngine::GeoTiledMappingManagerEngine(
    const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* error_string) :
    QGeoTiledMappingManagerEngine() {
    //! 初始化相机功能配置
    QGeoCameraCapabilities camera_caps;

    //! 设置缩放级别范围
    double minimumZoomLevel = 0;
    double maximumZoomLevel = 20;

    //! 配置相机基本功能
    camera_caps.setMinimumZoomLevel(minimumZoomLevel);
    camera_caps.setMaximumZoomLevel(maximumZoomLevel);
    camera_caps.setSupportsBearing(true); //! 支持方位角
    camera_caps.setSupportsTilting(true); //! 支持倾斜
    camera_caps.setMinimumTilt(0); //! 最小倾斜角
    camera_caps.setMaximumTilt(80); //! 最大倾斜角
    camera_caps.setMinimumFieldOfView(20.0); //! 最小视野角度
    camera_caps.setMaximumFieldOfView(120.0); //! 最大视野角度
    camera_caps.setOverzoomEnabled(true); //! 启用过度缩放
    setCameraCapabilities(camera_caps);

    //! 设置瓦片尺寸为标准的256x256像素
    setTileSize(QSize(256, 256));

    //! 初始化地图源，如果失败则返回错误
    if (!initializeMapSources(parameters, camera_caps)) {
        *error = QGeoServiceProvider::NotSupportedError;
        *error_string = Q_FUNC_INFO + QStringLiteral("initialize map sources error");
        return;
    }

    //! 创建并设置瓦片获取器
    GeoTileFetcher* tileFetcher = new GeoTileFetcher(this);
    setTileFetcher(tileFetcher);

    //! 缓存目录配置：优先使用参数指定的目录，否则使用默认缓存目录
    QString cacheDirectory;
    if (parameters.contains(QStringLiteral("map.cache.directory"))) {
        cacheDirectory = parameters.value(QStringLiteral("map.cache.directory")).toString();
    } else {
        cacheDirectory = QAbstractGeoTileCache::baseLocationCacheDirectory();
    }

    //! 创建并设置SQLite瓦片缓存
    SqliteTileCache* sqliteTileCache = new SqliteTileCache(cacheDirectory);
    setTileCache(sqliteTileCache);

    //! 预取样式配置：根据参数设置瓦片预取策略
    if (parameters.contains(QStringLiteral("map.prefetching_style"))) {
        const QString prefetchingMode =
            parameters.value(QStringLiteral("map.prefetching_style")).toString();
        if (prefetchingMode == QStringLiteral("TwoNeighbourLayers"))
            m_prefetchStyle = QGeoTiledMap::PrefetchTwoNeighbourLayers;
        else if (prefetchingMode == QStringLiteral("OneNeighbourLayer"))
            m_prefetchStyle = QGeoTiledMap::PrefetchNeighbourLayer;
        else if (prefetchingMode == QStringLiteral("NoPrefetching"))
            m_prefetchStyle = QGeoTiledMap::NoPrefetching;
    }

    //! 初始化成功，清除错误信息
    *error = QGeoServiceProvider::NoError;
    error_string->clear();
}

GeoTiledMappingManagerEngine::~GeoTiledMappingManagerEngine() {
    //! 析构函数：清理所有地图源对象
    qDeleteAll(_map_sources);
}

QGeoMap* GeoTiledMappingManagerEngine::createMap() {
    //! 创建新的瓦片地图实例
    QGeoTiledMap* map = new GeoTiledMap(this);

    //! 设置预取样式
    map->setPrefetchStyle(m_prefetchStyle);

    return map;
}

bool GeoTiledMappingManagerEngine::initializeMapSources(
    const QVariantMap& parameters, const QGeoCameraCapabilities& camera_caps) {
    //! 从资源文件中读取地图配置
    QFile maps_file(":/resources_url_config.json");

    if (!maps_file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray mapsData = maps_file.readAll();
    maps_file.close();

    //! 解析JSON配置文件
    QJsonParseError parse_error;
    QJsonDocument mapsDocument = QJsonDocument::fromJson(mapsData, &parse_error);
    if (!mapsDocument.isObject()) {
        return false;
    }

    //! 提取配置信息
    QVariantMap resources_url_config_js = mapsDocument.object().toVariantMap();
    QByteArray plugin_name = resources_url_config_js["pluginName"].toString().toLatin1();
    QVariantList map_sources = resources_url_config_js["mapSources"].toList();
    //! 遍历所有地图源配置
    for (const QVariant& mapSource_element : map_sources) {
        QVariantMap map_source = mapSource_element.toMap();
        QString provider = map_source["provider"].toString();
        QString copyright = map_source["copyright"].toString();
        QString coordinateSystem = map_source["coordinateSystem"].toString();
        //! 遍历每个地图源的所有样式
        QVariantList styles = map_source["styles"].toList();
        for (const QVariant& style_element : styles) {
            int mapId = _map_sources.count() + 1;
            QVariantMap style_map = style_element.toMap();
            QString style = style_map["style"].toString();
            QString description = provider + QStringLiteral(" ") + style;
            QString url = style_map["url"].toString();

            //! 创建地图源实例
            GeoMapSource* map_source = new GeoMapSource(
                provider,
                GeoMapSource::mapStyle(style),
                provider,
                description,
                false,
                false,
                mapId,
                plugin_name,
                url,
                copyright,
                camera_caps);

            //! 初始化访问令牌
            QString param = provider + ".token";
            if (parameters.contains(param)) {
                QString token = parameters.value(param).toString();
                map_source->setToken(token);
            }

            //! 设置服务器编号
            param = provider + ".serverId";
            if (parameters.contains(param)) {
                int serverId = parameters.value(param).toInt();
                map_source->setT(QString::number(serverId));
            }

            //! 设置地图版本
            param = provider + ".version";
            if (parameters.contains(param)) {
                int version = parameters.value(param).toInt();
                map_source->setG(QString::number(version));
            }

            //! 将地图源添加到列表中
            _map_sources.append(map_source);
        }
    }

    //! 创建支持的地图类型列表
    QList<QGeoMapType> map_types;
    for (GeoMapSource* source : _map_sources) {
        map_types << QGeoMapType(
            source->style(),
            source->name(),
            source->description(),
            source->mobile(),
            source->night(),
            source->mapId(),
            source->pluginName(),
            camera_caps);
    }

    //! 设置引擎支持的地图类型
    setSupportedMapTypes(map_types);
    return true;
}

GeoMapSource* GeoTiledMappingManagerEngine::mapSource(int mapId) const {
    //! 根据地图ID查找对应的地图源
    for (GeoMapSource* mapSource : mapSources()) {
        if (mapSource->mapId() == mapId)
            return mapSource;
    }

    return nullptr;
}
