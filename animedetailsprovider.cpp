#include "animedetailsprovider.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>

AnimeDetailsProvider::AnimeDetailsProvider(QObject *parent)
    : QObject(parent), m_networkManager(new QNetworkAccessManager(this)) {}

AnimeDetailsRequest *AnimeDetailsProvider::requestAnimeDetails(int malId) {
    AnimeDetailsRequest *req(new AnimeDetailsRequest(this));

    QNetworkReply *rep = m_networkManager->get(QNetworkRequest(
        QUrl(QString(R"(https://api.jikan.moe/v3/anime/%1)").arg(QString::number(malId)))));

    connect(rep, &QNetworkReply::finished, [req, rep]() {
        if (rep->error() != QNetworkReply::NoError)
            return;
        parseJsonDetails(req, rep->readAll());
    });
    connect(rep, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            [req, rep](QNetworkReply::NetworkError e) {
                req->setStatus(AnimeDetailsRequest::Status::Failed,
                               tr("Network Error - %1").arg(rep->errorString()));
            });
    connect(rep, &QNetworkReply::sslErrors, [rep]() { rep->ignoreSslErrors(); });
    return req;
}

void AnimeDetailsProvider::parseJsonDetails(AnimeDetailsRequest *rq, const QByteArray &data) {
    QJsonParseError err;
    QJsonDocument doc(QJsonDocument::fromJson(data, &err));
    if (err.error != QJsonParseError::NoError) {
        rq->setStatus(AnimeDetailsRequest::Failed, tr("Json Error %1").arg(err.errorString()));
        return;
    } else if (!doc.isObject()) {
        rq->setStatus(AnimeDetailsRequest::Failed,
                      tr("Json Error %1").arg("Body is not an object"));
        return;
    }

    QJsonObject obj(doc.object());
    qDebug() << obj;
    rq->setAnimeDetailsProp("malId", obj["mal_id"].toString());
    rq->setAnimeDetailsProp("title", obj["title"].toString());
    rq->setAnimeDetailsProp("englistTitle", obj["title_english"].toString());
    rq->setAnimeDetailsProp("japaneseTitle", obj["title_japanese"].toString());
    rq->setAnimeDetailsProp("source", obj["source"].toString());
    rq->setAnimeDetailsProp("episodes",obj["episodes"].toInt());
    rq->setAnimeDetailsProp("status", obj["status"].toString());
    rq->setAnimeDetailsProp("duration", obj["duration"].toString());
    rq->setAnimeDetailsProp("imageUrl", obj["image_url"].toString());
    rq->setAnimeDetailsProp("score", obj["score"].toDouble());
    rq->setAnimeDetailsProp("scoredBy", obj["scored_by"].toInt());
    rq->setAnimeDetailsProp("rank", obj["rank"].toInt());
    rq->setAnimeDetailsProp("popularity", obj["popularity"].toInt());
    rq->setAnimeDetailsProp("members", obj["members"].toInt());
    rq->setAnimeDetailsProp("broadcast", obj["broadcast"].toString());
    rq->setAnimeDetailsProp("premiered", obj["premiered"].toString());
    rq->setAnimeDetailsProp("duration", obj["duration"].toString());
    rq->setAnimeDetailsProp("synopsis", obj["synopsis"].toString());
    rq->setAnimeDetailsProp("background", obj["background"].toString());
    rq->setAnimeDetailsProp("rating", obj["rating"].toString());

    qDebug(__PRETTY_FUNCTION__);
    rq->setStatus(AnimeDetailsRequest::Completed);
}

AnimeDetails *AnimeDetailsRequest::details() const { return m_details; }
