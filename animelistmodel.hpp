#ifndef ANIMELISTMODEL_H
#define ANIMELISTMODEL_H

#include <QAbstractListModel>
#include "anime.hpp"

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

#endif // ANIMELISTMODEL_H
