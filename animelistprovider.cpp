#include "animelistprovider.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValueRef>

#include <QDebug>

AnimeListProvider::AnimeListProvider(QObject *parent)
    : AnimeRequest(parent), m_networkManager(new QNetworkAccessManager(this)) {}

const QStringList &AnimeListProvider::categoryList() {
    static const QStringList l = {"airing", "upcoming", "tv",           "movie",
                                  "ova",    "special",  "bypopularity", "favorite"};
    return l;
}

void AnimeListProvider::requestAnimeList(int page, int categoryIndex) {
    Q_ASSERT(!isLoading());
    Q_ASSERT(!m_currentRequest);

    qDebug() << "Page" << page << "catIndex" << categoryIndex;

    setStatus(RequestStatus::InProgress);
    m_currentRequest = m_networkManager->get(
        QNetworkRequest(QUrl(QString("https://api.jikan.moe/v3/top/anime/%1/%2")
                                 .arg(QString::number(page), categoryList()[categoryIndex]))));

    connect(m_currentRequest, &QNetworkReply::finished, [this]() {
        parseNetworkReply(m_currentRequest);
        m_currentRequest->deleteLater();
        m_currentRequest = nullptr;
    });
}

void AnimeListProvider::cancelCurrentRequest()
{
    if (!isLoading())
        return;
    Q_ASSERT(m_currentRequest);
    m_currentRequest->abort();
}

void AnimeListProvider::parseNetworkReply(QNetworkReply *r) try {
    if (r->error() != QNetworkReply::NoError) {
        throw tr("Network Error - %1").arg(r->errorString());
    }

    QJsonParseError parseErr;
    QJsonDocument reply = QJsonDocument::fromJson(r->readAll(), &parseErr);
    if (parseErr.error != QJsonParseError::NoError) {
        throw tr("Request Parse Error - %1").arg(parseErr.errorString());
    }

    auto object = reply.object();
    auto animeArray = object["top"].toArray();

    qDebug() << "got " << animeArray.size() << " animes";
    for (const auto &animeObject : animeArray) {
        if (!animeObject.isObject()) {
            throw tr("Request Parse Error - %1").arg("isn't an object");
            return;
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

    qDebug("done");
    setStatus(Completed);
} catch (const QString &err) {
    setStatus(Error, err);
}
