#include "animelistmodelprovider.hpp"
#include "anime.hpp"
#include "animelistprovider.hpp"

#include <QStandardItemModel>

#include <QDebug>

AnimeListModelProvider::AnimeListModelProvider(QObject *parent)
    : QObject(parent), m_animeListProvider(new AnimeListProvider(this)) {
    m_model = new AnimeListModel(this);

    connect(m_animeListProvider, &AnimeListProvider::gotAnime,
            [this](Anime a) { m_model->addAnime(std::move(a)); });
    connect(m_animeListProvider, &AnimeListProvider::animeListRequestDone,
            [this](bool failure, const QString &err) {
                if (failure) {
                    qDebug() << "Encountered error - " << err;
                }
                setGettingList(false);
            });

    setCategoryIndex(0);
}

QStringList AnimeListModelProvider::categoryList() {
    static const QStringList l = {"airing", "upcoming", "tv",           "movie",
                                  "ova",    "special",  "bypopularity", "favorite"};
    return l;
}

AnimeListModel *AnimeListModelProvider::model() const { return m_model; }

bool AnimeListModelProvider::gettingList() const { return m_gettingList; }

int AnimeListModelProvider::categoryIndex() const { return m_categoryIndex; }

void AnimeListModelProvider::setCategoryIndex(int categoryIndex) {
    if (gettingList() || m_categoryIndex == categoryIndex)
        return;

    m_categoryIndex = categoryIndex;
    emit categoryIndexChanged(m_categoryIndex);

    m_model->clear();
    setGettingList(true);
    m_animeListProvider->requestAnimeList(m_pageNumber, m_categoryIndex);
}

const QString &AnimeListModelProvider::category() const { return categoryList()[m_categoryIndex]; }

void AnimeListModelProvider::nextPage() {
    setGettingList(true);
    m_animeListProvider->requestAnimeList(++m_pageNumber, m_categoryIndex);
}

void AnimeListModel::clear() {
    beginResetModel();
    m_animes.clear();
    endResetModel();
}

Anime AnimeListModel::getAnimeById(int id) const {
    auto f = std::find_if(m_animes.cbegin(), m_animes.cend(),
                          [id](const Anime &a) { return a.malId() == id; });
    Q_ASSERT(f != m_animes.cend());
    return *f;
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

void AnimeListModelProvider::setGettingList(bool gettingList) {
    if (m_gettingList == gettingList)
        return;

    m_gettingList = gettingList;
    emit gettingListChanged(m_gettingList);
}
