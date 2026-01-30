#include "geocodereply.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGeoCoordinate>
#include <QGeoAddress>
#include <QGeoLocation>
#include <QGeoRectangle>

GeoCodeReply::GeoCodeReply(QNetworkReply* reply, OperationType operation_type, QObject* parent) :
    QGeoCodeReply(parent),
    _operation_type(operation_type) {
    if (!reply) {
        setError(UnknownError, QStringLiteral("Null reply"));
        return;
    }
    connect(reply, &QNetworkReply::finished, this, &GeoCodeReply::networkReplyFinished);
    connect(reply, &QNetworkReply::errorOccurred, this, &GeoCodeReply::networkReplyError);
    connect(this, &QGeoCodeReply::aborted, reply, &QNetworkReply::abort);
    connect(this, &QObject::destroyed, reply, &QObject::deleteLater);

    setLimit(1);
    setOffset(0);
}

GeoCodeReply::~GeoCodeReply() {}

void GeoCodeReply::networkReplyError(QNetworkReply::NetworkError error) {
    Q_UNUSED(error);
    QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();
    setError(QGeoCodeReply::CommunicationError, reply->errorString());
}

void GeoCodeReply::networkReplyFinished() {
    QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
        return;

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if (document.isObject()) {
        QJsonObject object = document.object();

        switch (operationType()) {
            case Geocode:
                break;
            case ReverseGeocode: {
                setLocations({parseAddress(object)});
                setFinished(true);
            } break;
        }

    } else {
        setError(QGeoCodeReply::CommunicationError, QStringLiteral("Unknown document"));
    }
}

//! 解析地址对象
/* 示例天地图反向地理编码结果JSON:{
    "result": {
        "formatted_address": "广东省江门市新会区大泽镇大井东北约101米",
        "location": {
            "lon": 112.944949,
            "lat": 22.535651
        },
        "addressComponent": {
            "address": "大井",
            "town": "大泽镇",
            "nation": "中国",
            "city": "江门市",
            "county_code": "156440705",
            "poi_position": "东北",
            "county": "新会区",
            "city_code": "156440700",
            "address_position": "东北",
            "poi": "大井",
            "province_code": "156440000",
            "town_code": "156440705101",
            "province": "广东省",
            "road": "Y191",
            "road_distance": 491,
            "address_distance": 101,
            "poi_distance": 101
        }
    },
    "msg": "ok",
    "status": "0"
}*/
QGeoLocation GeoCodeReply::parseAddress(const QJsonObject& object) {
    int status = object.value(QStringLiteral("status")).toInt();
    if (status != 0) {
        setError(QGeoCodeReply::ParseError, object.value(QStringLiteral("msg")).toString());
        return QGeoLocation();
    }
    QJsonObject resultObject = object.value(QStringLiteral("result")).toObject();

    QGeoAddress address;
    address.setText(resultObject.value(QStringLiteral("formatted_address")).toString());

    QJsonObject addressObject = resultObject.value(QStringLiteral("addressComponent")).toObject();
    address.setCountry(addressObject.value(QStringLiteral("county")).toString());
    address.setCountryCode(addressObject.value(QStringLiteral("country_code")).toString());
    address.setState(addressObject.value(QStringLiteral("province")).toString());
    address.setCity(addressObject.value(QStringLiteral("city")).toString());
    address.setDistrict(addressObject.value(QStringLiteral("town")).toString());
    address.setPostalCode(""); // 天地图反向地理编码结果中没有邮
    address.setStreet(addressObject.value(QStringLiteral("address")).toString());
    address.setStreetNumber(""); // 天地图反向地理编码结果中没有街道号码

    QGeoCoordinate coordinate;
    QJsonObject locationObject = resultObject.value(QStringLiteral("location")).toObject();

    coordinate.setLongitude(locationObject.value(QStringLiteral("lon")).toDouble());
    coordinate.setLatitude(locationObject.value(QStringLiteral("lat")).toDouble());

    QGeoLocation location;

    location.setCoordinate(coordinate);
    location.setAddress(address);

    return location;
}