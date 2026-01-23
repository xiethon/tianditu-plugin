#pragma once

#include <QtLocation/private/qgeofiletilecache_p.h>
#include <QtSql/QSqlDatabase>

class SqliteTileCache : public QGeoFileTileCache {
    Q_OBJECT
public:
    explicit SqliteTileCache(const QString& directory = QString(), QObject* parent = nullptr);
    ~SqliteTileCache();

    //! 根据瓦片规格从缓存中检索对应的瓦片纹理数据
    QSharedPointer<QGeoTileTexture> get(const QGeoTileSpec& spec) override;

    //! 将瓦片数据存储到SQLite缓存中，支持不同的缓存区域配置
    void insert(
        const QGeoTileSpec& spec,
        const QByteArray& bytes,
        const QString& format,
        QAbstractGeoTileCache::CacheAreas areas = QAbstractGeoTileCache::AllCaches) override;

private:
    //! 初始化数据库
    bool _initDatabase(const QString& directory);

    //! 从SQLite数据库中查询并返回指定瓦片的二进制数据
    QByteArray _getTileData(const QGeoTileSpec& spec);

    QSqlDatabase _database;
};
