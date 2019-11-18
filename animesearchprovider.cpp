#include "animesearchprovider.hpp"

#include <QJsonArray>
#include <QJsonDocument>

AnimeSearchProvider::AnimeSearchProvider(QNetworkAccessManager *i, QObject *parent)
    : AnimeListRequest(i, parent), m_model(new AnimeListModel(this)) {}

void AnimeSearchProvider::beginSearch() {
    if (isLoading()) {
        cancelCurrentRequest();
    }
    m_model->clear();
    m_page = 1;

    request(createRequestUrl());
}

void AnimeSearchProvider::nextPage() {
    if (isLoading() || isError())
        return;

    m_page++;

    request(createRequestUrl());
}

void AnimeSearchProvider::addAnime(Anime anime) { m_model->addAnime(std::move(anime)); }

void AnimeSearchProvider::parseJsonReply(const QJsonDocument &doc) {
    if (!doc.isObject())
        throw tr("Reply is not object");

    auto o = doc["results"];
    if (!o.isArray())
        throw tr("No results in Reply");
    jsonArrayToAnimes(o.toArray());
}

QString AnimeSearchProvider::keyWord() const { return m_keyWord; }

AnimeListModel *AnimeSearchProvider::model() const { return m_model; }

void AnimeSearchProvider::setKeyWord(QString keyWord) {
    if (m_keyWord == keyWord)
        return;

    m_keyWord = keyWord;
    emit keyWordChanged(m_keyWord);
}

QString toListParam(const QStringList &array, const QVector<int> &indices) {
    QString str;
    for (auto i = 0; i < indices.size() - 1; i++)
        str += array[i] + ',';
    str += array[indices.back()];
    return str;
}

QUrl AnimeSearchProvider::createRequestUrl() {

    QString url = "https://api.jikan.moe/v3/search/anime";
    QChar prefix = '?';
    auto addParameter = [&url, &prefix](const QString &param, const QString &arg) {
        url += prefix + param + '=' + arg;
        prefix = '&';
    };

    if (isValidKeyword())
        addParameter("q", QUrl::toPercentEncoding(m_keyWord));

    if (m_page != 1)
        addParameter("page", QString::number(m_page));

    if (m_typeIndices.size() > 0)
        addParameter("type", toListParam(types(), m_typeIndices));

    if (m_statusIndices.size() > 0)
        addParameter("status", toListParam(status(), m_statusIndices));

    if (m_ratingIndices.size() > 0)
        addParameter("rated", toListParam({"g", "pg", "pg13", "r17", "r", "rx"}, m_ratingIndices));

    static const QString orders[] = {"title",   "start_data", "end_date", "score", "type",
                                     "members", "id",         "episodes", "rating"};
    addParameter("order_by", orders[m_orderByIndex]);
    addParameter("sort", sort()[m_sortIndex]);

    if (m_genreIndices.size() > 0) {
        QString genreParam;
        for (auto i = 0; i < m_genreIndices.size() - 1; i++)
            genreParam += QString::number(m_genreIndices[i]) + ',';
        genreParam += QString::number(m_genreIndices.back());
        addParameter("genre", genreParam);
    }

    return url;
}

QStringList AnimeSearchProvider::types() {
    return {"tv", "ova", " movie", "special", "ona", "music"};
}

QStringList AnimeSearchProvider::status() { return {"airing", "completed", "upcoming"}; }

QStringList AnimeSearchProvider::rating() {
    return {"G - All Ages", "PG - Children", "PG - 13+", "R - 17+", "R + Mild Nudity", "Hentai"};
}

QStringList AnimeSearchProvider::orderBy() {
    return {"Title",  "Start - Date", "End - Data", "Score", "Type",
            "Member", "ID",           "Episodes",   "Rating"};
}

QStringList AnimeSearchProvider::sort() { return {"ascending", "descending"}; }

QStringList AnimeSearchProvider::genre() {
    return {
        "Action",     "Adventure", "Cars",          "Comedy",   "Dementia",      "Demons",
        "Mystry",     "Drama",     "Ecchi",         "Fantasy",  "Game",          "Hentai",
        "Historical", "Horror",    "Kids",          "Magic",    "Martial Arts",  "Mecha",
        "Music",      "Parody",    "Romance",       "School",   "Sci Fi",        "Shoujo",
        "Shoujon AI", "Shounen",   "Shounen AI",    "Space",    "Sports",        "Super Power",
        "Vampire",    "Yaoi",      "Yuri",          "Harem",    "Slice of Life", "Super Natural",
        "Military",   "Police",    "Psychological", "Thrillar", "Seinen",        "Josei"};
}

QVector<int> AnimeSearchProvider::typeIndices() const { return m_typeIndices; }

QVector<int> AnimeSearchProvider::statusIndices() const { return m_statusIndices; }

QVector<int> AnimeSearchProvider::ratingIndices() const { return m_ratingIndices; }

QVector<int> AnimeSearchProvider::genreIndices() const { return m_genreIndices; }

int AnimeSearchProvider::orderByIndex() const { return m_orderByIndex; }

int AnimeSearchProvider::sortIndex() const { return m_sortIndex; }

void AnimeSearchProvider::setTypeIndices(QVector<int> typeIndices) {
    if (m_typeIndices == typeIndices)
        return;

    m_typeIndices = typeIndices;
    emit typeIndicesChanged(m_typeIndices);
}

void AnimeSearchProvider::setStatusIndices(QVector<int> statusIndices) {
    if (m_statusIndices == statusIndices)
        return;

    m_statusIndices = statusIndices;
    emit statusIndicesChanged(m_statusIndices);
}

void AnimeSearchProvider::setRatingIndices(QVector<int> ratingIndices) {
    if (m_ratingIndices == ratingIndices)
        return;

    m_ratingIndices = ratingIndices;
    emit ratingIndicesChanged(m_ratingIndices);
}

void AnimeSearchProvider::setGenreIndices(QVector<int> genreIndices) {
    if (m_genreIndices == genreIndices)
        return;

    m_genreIndices = genreIndices;
    emit genreIndicesChanged(m_genreIndices);
}

void AnimeSearchProvider::setOrderByIndex(int orderByIndex) {
    if (m_orderByIndex == orderByIndex)
        return;

    m_orderByIndex = orderByIndex;
    emit orderByIndexChanged(m_orderByIndex);
}

void AnimeSearchProvider::setSortIndex(int sortIndex) {
    if (m_sortIndex == sortIndex)
        return;

    m_sortIndex = sortIndex;
    emit sortIndexChanged(m_sortIndex);
}
