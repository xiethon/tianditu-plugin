#pragma once

#include <QtLocation/private/qgeocameracapabilities_p.h>
#include <QtLocation/private/qgeomaptype_p.h>
#include <QtPositioning/QGeoCoordinate>

class QGeoTileSpec;

//! 地图数据源类
class GeoMapSource : public QGeoMapType {
public:
    GeoMapSource(
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
        const QGeoCameraCapabilities& camera_capabilities);

    static QGeoMapType::MapStyle mapStyle(const QString& style);

    QString url(const QGeoTileSpec& spec); //! 根据瓦片规格生成URL
    QString& copyright(); //! 获取版权信息
    QString& provider(); //! 获取供应商名称
    QString& token(); //! 获取访问token

    void setToken(const QString& token); //! 设置访问令牌
    QString t(); //! 获取服务器编号
    void setT(const QString& t); //! 设置服务器编号
    QString g(); //! 获取版本号信息
    void setG(const QString& g); //! 设置地图版本号

private:
    QString _provider{""};
    QString _token{""};
    QString _url{""};
    QString _copyright{""};
    QString _t{"0"};
    QString _g{"1"};
};
