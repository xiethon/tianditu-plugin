#include "geocodingmanagerengine.h"
#include "geocodereply.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QUrl>
#include <QUrlQuery>
#include <QLocale>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QGeoCoordinate>
#include <QGeoAddress>
#include <QGeoShape>
#include <QGeoRectangle>

//! 构造函数
GeoCodingManagerEngine::GeoCodingManagerEngine(
    const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* error_string) :
    QGeoCodingManagerEngine(parameters),
    _network_manager(new QNetworkAccessManager(this)) {
    _user_agent = QByteArrayLiteral("Qt Location application/1.0 (Linux; x86_64)");
    *error = QGeoServiceProvider::NoError;
    error_string->clear();
    init(parameters);
}

//! 初始化URL和访问令牌
void GeoCodingManagerEngine::init(const QVariantMap& parameters) {
    //! 从资源文件中读取地图配置
    QFile maps_file(":/resources_url_config.json");

    if (!maps_file.open(QIODevice::ReadOnly)) {
        return;
    }
    QByteArray mapsData = maps_file.readAll();
    maps_file.close();

    //! 解析JSON配置文件
    QJsonParseError parse_error;
    QJsonDocument mapsDocument = QJsonDocument::fromJson(mapsData, &parse_error);
    if (!mapsDocument.isObject()) {
        return;
    }

    //! 提取配置信息
    QVariantMap resources_url_config_js = mapsDocument.object().toVariantMap();
    _format_url = resources_url_config_js["geocode"].toString().toLatin1();

    QString plugin_name = resources_url_config_js["pluginName"].toString();
    QString param = plugin_name + ".token";
    if (parameters.contains(param)) {
        _token = parameters.value(param).toString();
    }
}

GeoCodingManagerEngine::~GeoCodingManagerEngine() {}

//! 反向地理编码方法: 根据地理坐标获取地址
QGeoCodeReply*
GeoCodingManagerEngine::reverseGeocode(const QGeoCoordinate& coordinate, const QGeoShape& bounds) {
    Q_UNUSED(bounds);

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::UserAgentHeader, _user_agent);
    QString url = _format_url;

    //! 替换token
    if (!_token.isEmpty() && url.contains("{token}")) {
        url.replace("{token}", _token);
    }

    //! 构造postStr参数
    auto postStr = QStringLiteral("{'lon':%1,'lat':%2,'ver':1}")
                       .arg(QLocale::c().toString(coordinate.longitude(), 'f', 6))
                       .arg(QLocale::c().toString(coordinate.latitude(), 'f', 6));
    //! 替换postStr
    if (url.contains("{postStr}")) {
        url.replace("{postStr}", postStr);
    }
    request.setUrl(QUrl(url));

    QNetworkReply* reply = _network_manager->get(request);
    GeoCodeReply* geocodeReply = new GeoCodeReply(reply, GeoCodeReply::ReverseGeocode, this);

    connect(geocodeReply, &GeoCodeReply::finished, this, &GeoCodingManagerEngine::replyFinished);
    connect(geocodeReply, &GeoCodeReply::errorOccurred, this, &GeoCodingManagerEngine::replyError);

    return geocodeReply;
}

//! 处理请求完成的槽函数
void GeoCodingManagerEngine::replyFinished() {
    QGeoCodeReply* reply = qobject_cast<QGeoCodeReply*>(sender());
    if (reply)
        emit finished(reply);
}

//! 处理请求错误的槽函数
void GeoCodingManagerEngine::replyError(
    QGeoCodeReply::Error errorCode, const QString& errorString) {
    QGeoCodeReply* reply = qobject_cast<QGeoCodeReply*>(sender());
    if (reply)
        emit errorOccurred(reply, errorCode, errorString);
}