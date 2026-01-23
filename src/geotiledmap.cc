//! 实现基于瓦片的地图显示功能，包括版权信息管理和瓦片渲染

#include "geotiledmap.h"
#include "geotiledmappingmanagerengine.h"

#include <QtLocation/private/qgeotilespec_p.h>

GeoTiledMap::GeoTiledMap(GeoTiledMappingManagerEngine* engine, QObject* parent) :
    QGeoTiledMap(engine, parent),
    _engine(engine),
    _map_id(-1) {}

GeoTiledMap::~GeoTiledMap() {}

void GeoTiledMap::evaluateCopyrights(const QSet<QGeoTileSpec>& visible_tiles) {
    if (visible_tiles.isEmpty()) {
        return;
    }

    QGeoTileSpec tile = *(visible_tiles.constBegin());
    //! 如果地图ID没有变化，无需更新版权信息
    if (tile.mapId() == _map_id) {
        return;
    }

    //! 如果地图源存在，发射版权信息变更信号
    _map_id = tile.mapId();
    GeoMapSource* mapSource = engine()->mapSource(_map_id);
    if (mapSource) {
        emit copyrightsChanged(mapSource->copyright());
    }
}
