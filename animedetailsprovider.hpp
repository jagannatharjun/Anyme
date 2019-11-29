#ifndef ANIMEDETAILSPROVIDER_HPP
#define ANIMEDETAILSPROVIDER_HPP

#include "animerequest.hpp"
#include <QObject>
#include <QQmlPropertyMap>

class QNetworkAccessManager;

class AnimeDetails : public QQmlPropertyMap {
    Q_OBJECT

public:
    AnimeDetails(QObject *parent = nullptr);
};

class AnimeDetailsRequest : public AnimeRequest {
    Q_OBJECT
public:
    AnimeDetailsRequest(QObject *parent = nullptr)
        : AnimeRequest(parent), m_details(new AnimeDetails(this)) {}

    friend class AnimeDetailsProvider;
    AnimeDetails *details() const;

private:
    AnimeDetails *m_details;
    void setAnimeDetailsProp(const char *prop, const QVariant &v) { m_details->insert(prop, v); }

private slots:
    void parseNetworkReply(class QNetworkReply *);
};

class AnimeDetailsProvider : public QObject {
    Q_OBJECT

public:
    AnimeDetailsProvider(QNetworkAccessManager *i, QObject *parent);
    AnimeDetailsRequest *requestAnimeDetails(int malId);

private:
    QNetworkAccessManager *m_networkManager;
};

#endif // ANIMEDETAILSPROVIDER_HPP
