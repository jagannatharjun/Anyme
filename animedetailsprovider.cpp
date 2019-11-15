#include "animedetailsprovider.hpp"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QQuickImageProvider>

#include <QHash>
#include <QPixmap>
#include <memory>

#include <QThread>

const QString qmlImageProvider = "animeDetailsImageProvider";

class AnimeDetailsImageProvider : public QQuickImageProvider {
public:
    AnimeDetailsImageProvider(QNetworkAccessManager *i)
        : QQuickImageProvider(QQuickImageProvider::Pixmap), m_networkManager(i) {}

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override {
        const auto &p = m_imageCache.value(id);
        if (size)
            *size = QSize(p.width(), p.height());
        return p.scaled(requestedSize.width() > 0 ? requestedSize.width() : p.width(),
                        requestedSize.height() > 0 ? requestedSize.height() : p.height());
    }

    QUrl cacheUrl(const QUrl &u, const QString &malId) {
        m_imageCache[malId] = download_from(u);
        return QUrl(
            QString("image://%1/%2").arg(qmlImageProvider, malId));
    }

private:
    QHash<QString, QPixmap> m_imageCache;
    QNetworkAccessManager *m_networkManager;

    QPixmap download_from(const QUrl &url) {
        QEventLoop loop;
        QNetworkReply *reply = m_networkManager->get(QNetworkRequest(url));
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        QPixmap pm;
        pm.loadFromData(reply->readAll());

        delete reply;
        return pm;
    }
};

AnimeDetailsProvider::AnimeDetailsProvider(QNetworkAccessManager *i, QObject *parent)
    : QObject(parent), m_networkManager(i),
      m_imageProvider(new AnimeDetailsImageProvider(m_networkManager)) {}

AnimeDetailsProvider::~AnimeDetailsProvider() { delete m_imageProvider; }

AnimeDetailsRequest *AnimeDetailsProvider::requestAnimeDetails(int malId) {
    AnimeDetailsRequest *req(new AnimeDetailsRequest(m_imageProvider, this));

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

QQuickImageProvider *AnimeDetailsProvider::imageProvider() { return m_imageProvider; }

QString AnimeDetailsProvider::imageProviderName() { return qmlImageProvider; }

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
    const auto malId = obj["mal_id"].toInt();
    setAnimeDetailsProp("imageUrl", m_imageProvider->cacheUrl(obj["image_url"].toString(), QString::number(malId)));
    setAnimeDetailsProp("malId", malId);
    setAnimeDetailsProp("title", obj["title"].toString());
    setAnimeDetailsProp("englistTitle", obj["title_english"].toString());
    setAnimeDetailsProp("japaneseTitle", obj["title_japanese"].toString());
    setAnimeDetailsProp("source", obj["source"].toString());
    setAnimeDetailsProp("episodes", obj["episodes"].toInt());
    setAnimeDetailsProp("status", obj["status"].toString());
    setAnimeDetailsProp("duration", obj["duration"].toString());
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
