#include "animelist.hpp"
#include "anime.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValueRef>
#include <QStandardItemModel>

#include <QDebug>

AnimeList::AnimeList(QObject *parent) : QObject(parent) {
    m_model = new AnimeModel(this);

    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &AnimeList::requestFinished);
}

QStringList AnimeList::categoryList() {
    static const QStringList l = {"airing", "upcoming", "tv",           "movie",
                                  "ova",    "special",  "bypopularity", "favorite"};
    return l;
}

void AnimeList::requestFinished(QNetworkReply *r) {
    Q_ASSERT(r->error() == QNetworkReply::NoError);
    QJsonParseError parseErr;
    QJsonDocument reply = QJsonDocument::fromJson(r->readAll(), &parseErr);
    Q_ASSERT(parseErr.error == QJsonParseError::NoError);

    auto object = reply.object();
    auto animeArray = object["top"].toArray();
    qDebug() << "got " << animeArray.size() << " animes";
    for (const auto &animeObject : animeArray) {
        Q_ASSERT(animeObject.isObject());
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

        qDebug() << newAnime.title() << newAnime.malId();
        m_model->addAnime(std::move(newAnime));
    }

    m_gettingList = false;
    emit gettingListChanged(m_gettingList);
}

AnimeModel *AnimeList::model() const { return m_model; }

bool AnimeList::gettingList() const { return m_gettingList; }

int AnimeList::categoryIndex() const { return m_categoryIndex; }

void AnimeList::setCategoryIndex(int categoryIndex) {
    if (gettingList() || m_categoryIndex == categoryIndex)
        return;
    qDebug() << "new index - " << categoryIndex;
    m_categoryIndex = categoryIndex;
    emit categoryIndexChanged(m_categoryIndex);

    m_model->clear();
    initGet(QString::number(m_pageNumber = 1), category());
}

void AnimeList::initGet(const QString page, const QString cat) {
    if (m_gettingList)
        return;

    m_gettingList = true;
    emit gettingListChanged(m_gettingList);

    m_networkManager->get(
        QNetworkRequest(QUrl(QString("https://api.jikan.moe/v3/top/anime/%1/%2").arg(page, cat))));
}


const QString &AnimeList::category() const { return categoryList()[m_categoryIndex]; }

void AnimeList::nextPage() { initGet(QString::number(++m_pageNumber), category()); }

void AnimeModel::clear() {
    beginResetModel();
    m_animes.clear();
    endResetModel();
}

void AnimeModel::addAnime(Anime anime) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_animes.push_back(std::move(anime));
    endInsertRows();
}

int AnimeModel::rowCount(const QModelIndex &) const { return m_animes.count(); }

QVariant AnimeModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= m_animes.count())
        return QVariant();

    const Anime &anime = m_animes[index.row()];
    switch (static_cast<AnimalRoles>(role)) {
    case RankRole:
        return anime.rank();
        break;
    case TitleRole:
        return anime.title();
        break;
    case UrlRole:
        return anime.url();
        break;
    case ImageUrlRole:
        return anime.imageUrl();
        break;
    case TypeRole:
        return anime.type();
        break;
    case EpisodesRole:
        return anime.episodes();
        break;
    case StartDateRole:
        return anime.startDate();
        break;
    case EndDateRole:
        return anime.endDate();
        break;
    case MembersRole:
        return anime.members();
        break;
    case ScoreRole:
        return anime.score();
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> AnimeModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[RankRole] = "rank";
    roles[TitleRole] = "title";
    roles[UrlRole] = "url";
    roles[ImageUrlRole] = "imageUrl";
    roles[TypeRole] = "type";
    roles[EpisodesRole] = "episodes";
    roles[StartDateRole] = "startDate";
    roles[EndDateRole] = "endDate";
    roles[MembersRole] = "members";
    roles[ScoreRole] = "score";
    return roles;
}
