#pragma once

#include <QGeoServiceProviderFactory>
#include <QObject>

//! 地理服务提供商工厂类
class GeoServiceProviderFactory : public QObject, public QGeoServiceProviderFactory {
    Q_OBJECT
    Q_INTERFACES(QGeoServiceProviderFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.qt.geoservice.serviceproviderfactory/6.0" FILE
                          "plugin.json")

public:
    QGeoMappingManagerEngine* createMappingManagerEngine(
        const QVariantMap& parameters,
        QGeoServiceProvider::Error* error,
        QString* error_string) const override;
};
