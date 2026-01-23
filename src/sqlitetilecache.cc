#include "sqlitetilecache.h"
#include <QDir>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

SqliteTileCache::SqliteTileCache(const QString& directory, QObject* parent) :
    QGeoFileTileCache(directory, parent) {
    //! 初始化数据库连接
    if (!_initDatabase(directory)) {
        qWarning() << "Failed to initialize SQLite database for tile cache";
    }
}

SqliteTileCache::~SqliteTileCache() {
    _database.close();
}

bool SqliteTileCache::_initDatabase(const QString& directory) {
    QDir dir;
    if (!dir.exists(directory)) {
        dir.mkpath(directory);
    }

    //! 设置数据库连接
    _database = QSqlDatabase::addDatabase("QSQLITE", "tile_cache_connection");
    _database.setDatabaseName(directory + "/map_cache.db");

    if (!_database.open()) {
        qWarning() << "Could not open tile cache database:" << _database.lastError();
        return false;
    }

    //! 创建表结构
    QSqlQuery query(_database);
    if (!query.exec("CREATE TABLE IF NOT EXISTS tiles ("
                    "zoom INTEGER, "
                    "x INTEGER, "
                    "y INTEGER, "
                    "map_id INTEGER, "
                    "format TEXT, "
                    "data BLOB, "
                    "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
                    "PRIMARY KEY (zoom, x, y, map_id))")) {
        qWarning() << "Could not create tiles table:" << query.lastError();
        return false;
    }

    return true;
}

void SqliteTileCache::insert(
    const QGeoTileSpec& spec,
    const QByteArray& bytes,
    const QString& format,
    QAbstractGeoTileCache::CacheAreas areas) {
    QSqlQuery query(_database);
    query.prepare("INSERT OR REPLACE INTO tiles (zoom, x, y, map_id, format, data) "
                  "VALUES (:zoom, :x, :y, :map_id, :format, :data)");
    query.bindValue(":zoom", spec.zoom());
    query.bindValue(":x", spec.x());
    query.bindValue(":y", spec.y());
    query.bindValue(":map_id", spec.mapId());
    query.bindValue(":format", format);
    query.bindValue(":data", bytes);

    if (!query.exec()) {
        qWarning() << "Failed to insert tile into database:" << query.lastError();
    }
}

QByteArray SqliteTileCache::_getTileData(const QGeoTileSpec& spec) {
    QSqlQuery query(_database);
    query.prepare(
        "SELECT data, format FROM tiles WHERE zoom = :zoom AND x = :x AND y = :y AND map_id = :map_id");
    query.bindValue(":zoom", spec.zoom());
    query.bindValue(":x", spec.x());
    query.bindValue(":y", spec.y());
    query.bindValue(":map_id", spec.mapId());

    if (query.exec() && query.next()) {
        return query.value(0).toByteArray();
    }

    return QByteArray();
}

QSharedPointer<QGeoTileTexture> SqliteTileCache::get(const QGeoTileSpec& spec) {
    QByteArray tile_data = _getTileData(spec);
    if (!tile_data.isEmpty()) {
        QImage image;
        if (image.loadFromData(tile_data)) {
            QSharedPointer<QGeoTileTexture> texture = QSharedPointer<QGeoTileTexture>::create();
            texture->image = image;
            texture->spec = spec;
            return texture;
        }
    }
    return QSharedPointer<QGeoTileTexture>();
}