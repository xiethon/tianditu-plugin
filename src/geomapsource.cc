#include "geomapsource.h"

#include <QUrl>
#include <QtLocation/private/qgeotilespec_p.h>

struct MapStyleData {
    QString name;
    QGeoMapType::MapStyle style;
};

static const MapStyleData mapStyles[] = {
    {QStringLiteral("StreetMap"), QGeoMapType::StreetMap},
    {QStringLiteral("SatelliteMapDay"), QGeoMapType::SatelliteMapDay},
    {QStringLiteral("SatelliteMapNight"), QGeoMapType::SatelliteMapNight},
    {QStringLiteral("TerrainMap"), QGeoMapType::TerrainMap},
    {QStringLiteral("HybridMap"), QGeoMapType::HybridMap},
    {QStringLiteral("TransitMap"), QGeoMapType::TransitMap},
    {QStringLiteral("GrayStreetMap"), QGeoMapType::GrayStreetMap},
    {QStringLiteral("PedestrianMap"), QGeoMapType::PedestrianMap},
    {QStringLiteral("CarNavigationMap"), QGeoMapType::CarNavigationMap},
    {QStringLiteral("CustomMap"), QGeoMapType::CustomMap}};

GeoMapSource::GeoMapSource(
    QString provider,
    QGeoMapType::MapStyle style,
    const QString& name,
    const QString& description,
    bool mobile,
    bool night,
    int mapId,
    const QByteArray& plugin_name,
    const QString& url,
    const QString& copyright,
    const QGeoCameraCapabilities& camera_capabilities) :
    QGeoMapType(style, name, description, mobile, night, mapId, plugin_name, camera_capabilities),
    _provider(provider),
    _url(url),
    _copyright(copyright) {}

QGeoMapType::MapStyle GeoMapSource::mapStyle(const QString& style_string) {
    for (const MapStyleData& mapStyle : mapStyles) {
        if (style_string.compare(mapStyle.name, Qt::CaseInsensitive) == 0)
            return mapStyle.style;
    }

    QGeoMapType::MapStyle style = static_cast<QGeoMapType::MapStyle>(style_string.toInt());

    if (style <= QGeoMapType::NoMap)
        style = QGeoMapType::CustomMap;

    return style;
}

QString& GeoMapSource::provider() {
    return _provider;
}

QString& GeoMapSource::token() {
    return _token;
}

QString GeoMapSource::url(const QGeoTileSpec& spec) {
    int zoom = spec.zoom();
    int x = spec.x();
    int y = spec.y();

    QString format_url = _url;

    //! 替换访问令牌
    if (!token().isEmpty() && format_url.contains(QLatin1String("{token}"))) {
        format_url.replace(QLatin1String("{token}"), token());
    }

    //! 替换g参数,用于服务器选择
    if (!g().isEmpty() && format_url.contains(QLatin1String("{g}"))) {
        format_url.replace(QLatin1String("{g}"), g());
    }

    //! 替换t参数,用于版本选择
    if (!t().isEmpty() && format_url.contains(QLatin1String("{t}"))) {
        format_url.replace(QLatin1String("{t}"), t());
    }

    //! QuadKey编码：将瓦片坐标转换为四叉树键值
    if (format_url.contains(QLatin1String("{quadkey}"))) {
        QString quadkey;
        for (int i = zoom; i > 0; --i) {
            int mask = 1 << (i - 1);
            int cell = ((x & mask) != 0 ? 1 : 0) + ((y & mask) != 0 ? 2 : 0);
            quadkey.append(QString::number(cell));
        }
        format_url.replace(QLatin1String("{quadkey}"), quadkey);
    } else {
        //! 标准瓦片坐标替换：z(缩放级别)、x(列)、y(行)
        format_url.replace(QLatin1String("{z}"), QString::number(zoom));
        format_url.replace(QLatin1String("{x}"), QString::number(x));
        format_url.replace(QLatin1String("{y}"), QString::number(y));
    }
    return format_url;
}

//! 返回版权信息
QString& GeoMapSource::copyright() {
    return _copyright;
}

//! 设置访问令牌
void GeoMapSource::setToken(const QString& token) {
    _token = token;
}

//! 返回t参数,版本标识
QString GeoMapSource::t() {
    return _t;
}

//! 设置t参数
void GeoMapSource::setT(const QString& t) {
    _t = t;
}

//! 返回g参数
QString GeoMapSource::g() {
    return _g;
}

//! 设置g参数
void GeoMapSource::setG(const QString& g) {
    _g = g;
}