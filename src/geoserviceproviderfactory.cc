#include "geoserviceproviderfactory.h"
#include <QtLocation/private/qgeotiledmappingmanagerengine_p.h>
#include "geotiledmappingmanagerengine.h"

QGeoMappingManagerEngine* GeoServiceProviderFactory::createMappingManagerEngine(
    const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* error_string) const {
    return new GeoTiledMappingManagerEngine(parameters, error, error_string);
}
