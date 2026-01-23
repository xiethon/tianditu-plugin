//! 地理瓦片获取器类,负责从网络获取地图瓦片数据,管理网络请求的发送、用户代理设置和访问令牌配置

#pragma once

#include <QtLocation/private/qgeotilefetcher_p.h>

class QGeoTiledMappingManagerEngine;
class QNetworkAccessManager;

class GeoTileFetcher : public QGeoTileFetcher {
    Q_OBJECT

public:
    explicit GeoTileFetcher(QGeoTiledMappingManagerEngine* parent);

    const QByteArray& userAgent() const;
    void setUserAgent(const QByteArray& user_agent);
    const QString& token() const;
    void setToken(const QString& token);

private:
    QGeoTiledMapReply* getTileImage(const QGeoTileSpec& spec) override; //! 获取瓦片图像

    QNetworkAccessManager* _network_manager;
    QByteArray _user_agent;
    QString _token;
};
