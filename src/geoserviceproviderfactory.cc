#include "geoserviceproviderfactory.h"
#include <QtLocation/private/qgeotiledmappingmanagerengine_p.h>
#include "geotiledmappingmanagerengine.h"
#include "geocodingmanagerengine.h"

#include <QtLocation/private/qgeotiledmappingmanagerengine_p.h>

QGeoMappingManagerEngine* GeoServiceProviderFactory::createMappingManagerEngine(
    const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* error_string) const {
    return new GeoTiledMappingManagerEngine(parameters, error, error_string);
}

QGeoCodingManagerEngine* GeoServiceProviderFactory::createGeocodingManagerEngine(
    const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* error_string) const {
    return new GeoCodingManagerEngine(parameters, error, error_string);
}
