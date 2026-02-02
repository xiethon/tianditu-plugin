#pragma once

#include <QGeoServiceProvider>
#include <QGeoCodingManagerEngine>
#include <QGeoCodeReply>

class QNetworkAccessManager;

//! 地理编码管理引擎类
class GeoCodingManagerEngine : public QGeoCodingManagerEngine {
    Q_OBJECT

public:
    GeoCodingManagerEngine(
        const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* error_string);
    virtual ~GeoCodingManagerEngine();

    //! 地理编码方法: 根据地址获取地理坐标
    QGeoCodeReply* geocode(const QGeoAddress& address, const QGeoShape& bounds) override;
    QGeoCodeReply*
    geocode(const QString& address, int limit, int offset, const QGeoShape& bounds) override;

    //! 反向地理编码方法: 根据地理坐标获取地址
    QGeoCodeReply*
    reverseGeocode(const QGeoCoordinate& coordinate, const QGeoShape& bounds) override;

    QString getReverseGeoCodePostStr(const QGeoCoordinate& coordinate);
    QString getGeocodePostStr(const QString& keyword, int limit = 5, int query_type = 1);
private Q_SLOTS:
    void replyFinished();
    void replyError(QGeoCodeReply::Error error_code, const QString& error_string);

private:
    void init(const QVariantMap& parameters);

private:
    QNetworkAccessManager* _network_manager;
    QByteArray _user_agent;
    QString _geocode_url;
    QString _reverse_geocode_url;
    QString _token;
};