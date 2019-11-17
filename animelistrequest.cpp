#include "animelistrequest.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValueRef>

#include <QDebug>

AnimeListRequest::AnimeListRequest(QNetworkAccessManager *i, QObject *parent)
    : AnimeRequest(parent), m_networkManager(i) {}

void AnimeListRequest::request(const QUrl &url) {
    Q_ASSERT(!isLoading());
    Q_ASSERT(!m_currentRequest);

    setStatus(RequestStatus::InProgress);
    m_currentRequest = m_networkManager->get(QNetworkRequest(url));

    connect(m_currentRequest, &QNetworkReply::finished, [this]() {
        parseNetworkReply(m_currentRequest);
        m_currentRequest->deleteLater();
        m_currentRequest = nullptr;
    });
}

void AnimeListRequest::cancelCurrentRequest() {
    if (!isLoading())
        return;
    Q_ASSERT(m_currentRequest);
    m_currentRequest->abort();
}

void AnimeListRequest::jsonArrayToAnimes(const QJsonArray &animeArray) {
    for (const auto &animeObject : animeArray) {
        if (!animeObject.isObject()) {
            throw tr("Request Parse Error - %1").arg("isn't an object");
        }

        auto obj = animeObject.toObject();
        Anime newAnime;
        newAnime.setMalId(obj["mal_id"].toInt());
        newAnime.setRank(obj["rank"].toInt());
        newAnime.setTitle(obj["title"].toString());
        newAnime.setUrl(QUrl(obj["url"].toString()));
        newAnime.setImageurl(QUrl(obj["image_url"].toString()));
        newAnime.setType(obj["type"].toString());
        newAnime.setEpisodes(obj["episoded"].toInt());
        newAnime.setStartdate(QDate::fromString(obj["start_date"].toString(), "MMM YYYY"));
        newAnime.setEnddate(QDate::fromString(obj["end_date"].toString(), "MMM YYYY"));
        newAnime.setMembers(obj["members"].toInt());
        newAnime.setScore(obj["score"].toDouble());

        addAnime(std::move(newAnime));
    }
}

void AnimeListRequest::parseNetworkReply(QNetworkReply *r) try {
    if (r->error() != QNetworkReply::NoError) {
        throw tr("Network Error - %1").arg(r->errorString());
    }

    QJsonParseError parseErr;
    QJsonDocument reply = QJsonDocument::fromJson(r->readAll(), &parseErr);
    if (parseErr.error != QJsonParseError::NoError) {
        throw tr("Request Parse Error - %1").arg(parseErr.errorString());
    }

    parseJsonReply(reply);

    setStatus(Completed);
} catch (const QString &err) {
    setStatus(Error, err);
}
