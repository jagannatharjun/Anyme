#include "animelistmodelprovider.hpp"
#include <QDebug>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

AnimeListModelProvider::AnimeListModelProvider(QNetworkAccessManager *i, QObject *parent)
    : AnimeListRequest(i, parent) {
    m_model = new AnimeListModel(this);
}

QStringList AnimeListModelProvider::categoryList() {
    static const QStringList l = {"airing", "upcoming", "tv",           "movie",
                                  "ova",    "special",  "bypopularity", "favorite"};
    return l;
}

AnimeListModel *AnimeListModelProvider::model() const { return m_model; }

int AnimeListModelProvider::categoryIndex() const { return m_categoryIndex; }

void AnimeListModelProvider::setCategoryIndex(int categoryIndex) {
    cancelCurrentRequest();
    Q_ASSERT(!isLoading());

    m_categoryIndex = categoryIndex;

    m_model->clear();
    m_pageNumber = 1;

    requestAnimeList(m_pageNumber, m_categoryIndex);

    emit categoryIndexChanged(m_categoryIndex);
}

const QString &AnimeListModelProvider::category() const { return categoryList()[m_categoryIndex]; }

void AnimeListModelProvider::nextPage() {
    if (isLoading())
        return;

    requestAnimeList(++m_pageNumber, m_categoryIndex);
}

void AnimeListModelProvider::addAnime(Anime a) { m_model->addAnime(a); }

void AnimeListModelProvider::requestAnimeList(int page, int categoryIndex) {
    request(QUrl(QString("https://api.jikan.moe/v3/top/anime/%1/%2")
                     .arg(QString::number(page), categoryList()[categoryIndex])));
}

void AnimeListModelProvider::parseJsonReply(const QJsonDocument &doc) {
    if (!doc.isObject())
        throw tr("Reply is not object");
    auto o = doc["top"];
    if (!o.isArray())
        throw tr("No top in Reply");
    jsonArrayToAnimes(o.toArray());
}
