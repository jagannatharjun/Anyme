#ifndef ANIMELIST_HPP
#define ANIMELIST_HPP

#include <QAbstractItemModel>
#include <QObject>

#include "anime.hpp"
#include "animelistprovider.hpp"
#include "animerequest.hpp"

class AnimeListProvider;

class AnimeListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum AnimeRoles {
        MalIdRole = Qt::UserRole + 1,
        RankRole,
        TitleRole,
        UrlRole,
        ImageUrlRole,
        TypeRole,
        EpisodesRole,
        StartDateRole,
        EndDateRole,
        MembersRole,
        ScoreRole,
    };

    void clear();
    int size() const { return m_animes.size(); }

    AnimeListModel(QObject *parent = 0) : QAbstractListModel(parent) {}
    void addAnime(Anime anime);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QVector<Anime> m_animes;
};

class AnimeListModelProvider : public AnimeListProvider {
    Q_OBJECT
    Q_PROPERTY(AnimeListModel *model READ model)
    Q_PROPERTY(
        int categoryIndex READ categoryIndex WRITE setCategoryIndex NOTIFY categoryIndexChanged)

public:
    explicit AnimeListModelProvider(QObject *parent = nullptr);

    Q_INVOKABLE static QStringList categoryList();

    int categoryIndex() const;
    void setCategoryIndex(int categoryIndex);
    const QString &category() const;

    Q_INVOKABLE void nextPage();
    AnimeListModel *model() const;

signals:
    void categoryIndexChanged(int categoryIndex);

private:
    AnimeListModel *m_model;
    int m_categoryIndex = 0;
    int m_pageNumber = 1;

    void addAnime(Anime a) override;
};

#endif // ANIMELIST_HPP
