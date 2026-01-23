#include "geotiledmapreply.h"
#include <QtLocation/private/qgeotilespec_p.h>

static const unsigned char pngSignature[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00};
static const unsigned char jpegSignature[] = {0xFF, 0xD8, 0xFF, 0x00};
static const unsigned char gifSignature[] = {0x47, 0x49, 0x46, 0x38, 0x00};

GeoTiledMapReply::GeoTiledMapReply(
    QNetworkReply* reply, const QGeoTileSpec& spec, QObject* parent) :
    QGeoTiledMapReply(spec, parent) {
    if (!reply) {
        setError(UnknownError, QStringLiteral("Null reply"));
        return;
    }
    connect(reply, &QNetworkReply::finished, this, &GeoTiledMapReply::_networkReplyFinished);
    connect(reply, &QNetworkReply::errorOccurred, this, &GeoTiledMapReply::_networkReplyError);
    connect(this, &QGeoTiledMapReply::aborted, reply, &QNetworkReply::abort);
    connect(this, &QObject::destroyed, reply, &QObject::deleteLater);
}

GeoTiledMapReply::~GeoTiledMapReply() {}

void GeoTiledMapReply::_networkReplyFinished() {
    QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
        return;

    QByteArray const& image_data = reply->readAll();

    bool _valid_format = true;
    if (image_data.startsWith(reinterpret_cast<const char*>(pngSignature)))
        setMapImageFormat(QStringLiteral("png"));
    else if (image_data.startsWith(reinterpret_cast<const char*>(jpegSignature)))
        setMapImageFormat(QStringLiteral("jpg"));
    else if (image_data.startsWith(reinterpret_cast<const char*>(gifSignature)))
        setMapImageFormat(QStringLiteral("gif"));
    else
        _valid_format = false;

    if (_valid_format)
        setMapImageData(image_data);

    setFinished(true);
}

void GeoTiledMapReply::_networkReplyError(QNetworkReply::NetworkError error) {
    QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();
    if (error == QNetworkReply::OperationCanceledError)
        setFinished(true);
    else
        setError(QGeoTiledMapReply::CommunicationError, reply->errorString());
}
