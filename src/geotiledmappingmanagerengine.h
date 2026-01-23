
#pragma once
#include <QGeoServiceProvider>
#include <QtLocation/private/qgeotiledmappingmanagerengine_p.h>

#include "geomapsource.h"

//! 地理瓦片地图管理引擎类
class GeoTiledMappingManagerEngine : public QGeoTiledMappingManagerEngine {
    Q_OBJECT

public:
    GeoTiledMappingManagerEngine(
        const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* error_string);

    virtual ~GeoTiledMappingManagerEngine();

    QGeoMap* createMap() override;
    inline const QList<GeoMapSource*>& mapSources() const;
    GeoMapSource* mapSource(int map_id) const;

private:
    bool
    initializeMapSources(const QVariantMap& parameters, const QGeoCameraCapabilities& camera_caps);
    QList<GeoMapSource*> _map_sources;
};

inline const QList<GeoMapSource*>& GeoTiledMappingManagerEngine::mapSources() const {
    return _map_sources;
}
