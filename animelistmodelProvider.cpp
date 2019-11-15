#include "animelistmodelprovider.hpp"
#include "anime.hpp"
#include "animelistprovider.hpp"

#include <QStandardItemModel>

#include <QDebug>

AnimeListModelProvider::AnimeListModelProvider(QNetworkAccessManager *i, QObject *parent) : AnimeListProvider(i, parent) {
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

void AnimeListModel::clear() {
    beginResetModel();
    m_animes.clear();
    endResetModel();
}

void AnimeListModel::addAnime(Anime anime) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_animes.push_back(std::move(anime));
    endInsertRows();
}

int AnimeListModel::rowCount(const QModelIndex &) const { return m_animes.count(); }

QVariant AnimeListModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= m_animes.count())
        return QVariant();

    const Anime &anime = m_animes[index.row()];
    switch (static_cast<AnimeRoles>(role)) {
    case MalIdRole:
        return anime.malId();
    case RankRole:
        return anime.rank();
    case TitleRole:
        return anime.title();
    case UrlRole:
        return anime.url();
    case ImageUrlRole:
        return anime.imageUrl();
    case TypeRole:
        return anime.type();
    case EpisodesRole:
        return anime.episodes();
    case StartDateRole:
        return anime.startDate();
        break;
    case EndDateRole:
        return anime.endDate();
    case MembersRole:
        return anime.members();
    case ScoreRole:
        return anime.score();
    }

    return QVariant();
}

QHash<int, QByteArray> AnimeListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[MalIdRole] = "malId";
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
