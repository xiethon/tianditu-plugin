#pragma once

#include <QNetworkReply>
#include <QGeoCodeReply>

class QGeoLocation;

class GeoCodeReply : public QGeoCodeReply {
    Q_OBJECT
    Q_MOC_INCLUDE(<QGeoLocation>)

public:
    enum OperationType { Geocode, ReverseGeocode };

public:
    GeoCodeReply(QNetworkReply* reply, OperationType operation_type, QObject* parent = nullptr);
    ~GeoCodeReply();

    //! 获取操作类型
    inline OperationType operationType() const;

private Q_SLOTS:
    //! 处理网络请求完成的槽函数
    void networkReplyFinished();
    void networkReplyError(QNetworkReply::NetworkError error);

    //! 解析地址对象
    QGeoLocation parseReverseReply(const QJsonObject& object);
    QList<QGeoLocation> parseReply(const QJsonObject& object);

private:
    OperationType _operation_type;
};

//! 获取操作类型
inline GeoCodeReply::OperationType GeoCodeReply::operationType() const {
    return _operation_type;
}