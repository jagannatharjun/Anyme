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
    : QObject(parent), m_networkManager(new QNetworkAccessManager(this)) {
    connect(m_networkManager, &QNetworkAccessManager::finished, this,
            &AnimeListProvider::parseAnimeList);
}

const QStringList &AnimeListProvider::categoryList() {
    static const QStringList l = {"airing", "upcoming", "tv",           "movie",
                                  "ova",    "special",  "bypopularity", "favorite"};
    return l;
}

void AnimeListProvider::requestAnimeList(int page, int categoryIndex) {
    qDebug(__PRETTY_FUNCTION__);
    m_networkManager->get(
        QNetworkRequest(QUrl(QString("https://api.jikan.moe/v3/top/anime/%1/%2")
                                 .arg(QString::number(page), categoryList()[categoryIndex]))));
}

void AnimeListProvider::parseAnimeList(QNetworkReply *r) {
    if (r->error() != QNetworkReply::NoError) {
        emit animeListRequestDone(true, tr("Network Error - %1").arg(r->errorString()));
        return;
    }

    QJsonParseError parseErr;
    QJsonDocument reply = QJsonDocument::fromJson(r->readAll(), &parseErr);
    if (parseErr.error != QJsonParseError::NoError) {
        emit animeListRequestDone(true, tr("Request Parse Error - %1").arg(parseErr.errorString()));
        return;
    }

    auto object = reply.object();
    auto animeArray = object["top"].toArray();
    qDebug() << "got " << animeArray.size() << " animes";
    for (const auto &animeObject : animeArray) {
        if (!animeObject.isObject()) {
            emit animeListRequestDone(true, tr("Request Parse Error - %1").arg("isn't an object"));
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

        emit gotAnime(std::move(newAnime));
    }

    animeListRequestDone(false, "");
}
