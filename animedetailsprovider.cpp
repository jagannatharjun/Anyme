#include "animedetailsprovider.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>

AnimeDetailsProvider::AnimeDetailsProvider(QObject *parent)
    : QObject(parent), m_networkManager(new QNetworkAccessManager(this)) {}

AnimeDetailsRequest *AnimeDetailsProvider::requestAnimeDetails(int malId) {
    AnimeDetailsRequest *req(new AnimeDetailsRequest(this));

    req->setStatus(AnimeRequest::InProgress);

    QNetworkReply *rep = m_networkManager->get(QNetworkRequest(
        QUrl(QString(R"(https://api.jikan.moe/v3/anime/%1)").arg(QString::number(malId)))));

    connect(rep, &QNetworkReply::finished, [req, rep]() {
        req->parseNetworkReply(rep);
        rep->deleteLater();
    });
    connect(rep, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            [req, rep](QNetworkReply::NetworkError) {
                req->setStatus(AnimeRequest::RequestStatus::Error,
                               tr("Network Error - %1").arg(rep->errorString()));
            });
    connect(rep, &QNetworkReply::sslErrors, [rep]() { rep->ignoreSslErrors(); });
    return req;
}

void AnimeDetailsRequest::parseNetworkReply(QNetworkReply *r) try {
    if (r->error() != QNetworkReply::NoError)
        throw tr("Network Reply - %1").arg(r->errorString());

    QJsonParseError err;
    QJsonDocument doc(QJsonDocument::fromJson(r->readAll(), &err));
    if (err.error != QJsonParseError::NoError) {
        throw tr("Json Error %1").arg(err.errorString());
    } else if (!doc.isObject()) {
        throw tr("Json Error %1").arg("Body is not an object");
    }

    QJsonObject obj(doc.object());
    setAnimeDetailsProp("malId", obj["mal_id"].toString());
    setAnimeDetailsProp("title", obj["title"].toString());
    setAnimeDetailsProp("englistTitle", obj["title_english"].toString());
    setAnimeDetailsProp("japaneseTitle", obj["title_japanese"].toString());
    setAnimeDetailsProp("source", obj["source"].toString());
    setAnimeDetailsProp("episodes", obj["episodes"].toInt());
    setAnimeDetailsProp("status", obj["status"].toString());
    setAnimeDetailsProp("duration", obj["duration"].toString());
    setAnimeDetailsProp("imageUrl", obj["image_url"].toString());
    setAnimeDetailsProp("score", obj["score"].toDouble());
    setAnimeDetailsProp("scoredBy", obj["scored_by"].toInt());
    setAnimeDetailsProp("rank", obj["rank"].toInt());
    setAnimeDetailsProp("popularity", obj["popularity"].toInt());
    setAnimeDetailsProp("members", obj["members"].toInt());
    setAnimeDetailsProp("broadcast", obj["broadcast"].toString());
    setAnimeDetailsProp("premiered", obj["premiered"].toString());
    setAnimeDetailsProp("duration", obj["duration"].toString());
    setAnimeDetailsProp("synopsis", obj["synopsis"].toString());
    setAnimeDetailsProp("background", obj["background"].toString());
    setAnimeDetailsProp("rating", obj["rating"].toString());

    setStatus(AnimeRequest::RequestStatus::Completed);

} catch (const QString &err) {
    setStatus(AnimeRequest::RequestStatus::Error, err);
}

AnimeDetails *AnimeDetailsRequest::details() const { return m_details; }

AnimeDetails::AnimeDetails(QObject *parent) : QQmlPropertyMap(parent) {}
