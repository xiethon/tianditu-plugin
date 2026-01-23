
#pragma once

#include <QtLocation/private/qgeotiledmap_p.h>

class GeoTiledMappingManagerEngine;

//! 瓦片地图类，继承自QGeoTiledMap。负责管理地图瓦片的加载、显示和版权信息处理
class GeoTiledMap : public QGeoTiledMap {
    Q_OBJECT

public:
    explicit GeoTiledMap(GeoTiledMappingManagerEngine* engine, QObject* parent = nullptr);
    virtual ~GeoTiledMap();

protected:
    void evaluateCopyrights(const QSet<QGeoTileSpec>& visible_tiles) override; //! 版权信息
    inline GeoTiledMappingManagerEngine* engine() const; //! 获取地图管理引擎

private:
    GeoTiledMappingManagerEngine* _engine; //! 地图管理引擎指针
    int _map_id;
};

inline GeoTiledMappingManagerEngine* GeoTiledMap::engine() const {
    return _engine;
}
