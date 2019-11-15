#ifndef ANIMEDETAILSPROVIDER_HPP
#define ANIMEDETAILSPROVIDER_HPP

#include "animerequest.hpp"
#include <QObject>
#include <QQmlPropertyMap>

class QNetworkAccessManager;
class QQuickImageProvider;
class AnimeDetailsImageProvider;

class AnimeDetails : public QQmlPropertyMap {
    Q_OBJECT

public:
    AnimeDetails(QObject *parent = nullptr);
};

class AnimeDetailsRequest : public AnimeRequest {
public:
    AnimeDetailsRequest(AnimeDetailsImageProvider *img, QObject *parent = nullptr)
        : AnimeRequest(parent), m_details(new AnimeDetails(this)), m_imageProvider(img) {}

    friend class AnimeDetailsProvider;
    AnimeDetails *details() const;

private:
    AnimeDetails *m_details;
    AnimeDetailsImageProvider *m_imageProvider;
    void setAnimeDetailsProp(const char *prop, const QVariant &v) { m_details->insert(prop, v); }

private:
    void parseNetworkReply(class QNetworkReply *) override;
};

class AnimeDetailsProvider : public QObject {
    Q_OBJECT

public:
    AnimeDetailsProvider(QNetworkAccessManager *i, QObject *parent);
    ~AnimeDetailsProvider();
    AnimeDetailsRequest *requestAnimeDetails(int malId);
    QQuickImageProvider *imageProvider();
    QString imageProviderName();

private:
    QNetworkAccessManager *m_networkManager;
    AnimeDetailsImageProvider *m_imageProvider;
};

#endif // ANIMEDETAILSPROVIDER_HPP
