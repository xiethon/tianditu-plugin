//! 地图瓦片请求回复类,将网络层的QNetworkReply与地图瓦片系统进行桥接，确保瓦片数据能够正确地从网络传输到地图渲染系统。

#pragma once
#include <QNetworkReply>
#include <QtLocation/private/qgeotiledmapreply_p.h>

class GeoTiledMapReply : public QGeoTiledMapReply {
    Q_OBJECT

public:
    //! 创建一个地图瓦片回复对象，关联网络回复和瓦片规格
    GeoTiledMapReply(QNetworkReply* reply, const QGeoTileSpec& spec, QObject* parent = nullptr);
    ~GeoTiledMapReply();

private Q_SLOTS:
    void _networkReplyFinished();
    void _networkReplyError(QNetworkReply::NetworkError error);
};
