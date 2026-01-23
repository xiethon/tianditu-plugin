
#include "geotilefetcher.h"
#include "geotiledmappingmanagerengine.h"
#include "geotiledmapreply.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QtLocation/private/qgeotilespec_p.h>

GeoTileFetcher::GeoTileFetcher(QGeoTiledMappingManagerEngine* parent) :
    QGeoTileFetcher(parent),
    _network_manager(new QNetworkAccessManager(this)),
    _user_agent(QByteArrayLiteral("Qt Location application/1.0 (Linux; x86_64)")) {}

QGeoTiledMapReply* GeoTileFetcher::getTileImage(const QGeoTileSpec& spec) {
    QNetworkRequest request;
    request.setRawHeader("User-Agent", userAgent());

    GeoTiledMappingManagerEngine* engine = qobject_cast<GeoTiledMappingManagerEngine*>(parent());
    GeoMapSource* mapSource = engine->mapSource(spec.mapId());
    if (mapSource) {
        QString url = mapSource->url(spec);
        request.setUrl(url);
    }

    QNetworkReply* reply = _network_manager->get(request);
    return new GeoTiledMapReply(reply, spec);
}

const QByteArray& GeoTileFetcher::userAgent() const {
    return _user_agent;
}

void GeoTileFetcher::setUserAgent(const QByteArray& user_agent) {
    _user_agent = user_agent;
}

const QString& GeoTileFetcher::token() const {
    return _token;
}

void GeoTileFetcher::setToken(const QString& token) {
    _token = token;
}
