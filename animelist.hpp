#ifndef ANIMELIST_HPP
#define ANIMELIST_HPP

#include <QAbstractItemModel>
#include <QObject>

#include "anime.hpp"

class QNetworkReply;
class QNetworkAccessManager;

class AnimeModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum AnimalRoles {
        RankRole = Qt::UserRole + 1,
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
    AnimeModel(QObject *parent = 0) : QAbstractListModel(parent) {}
    void addAnime(Anime anime);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QVector<Anime> m_animes;
};

class AnimeModel;

class AnimeList : public QObject {
    Q_OBJECT
    Q_PROPERTY(AnimeModel *model READ model)
    Q_PROPERTY(int categoryIndex READ categoryIndex WRITE setCategoryIndex NOTIFY categoryIndexChanged)
    Q_PROPERTY(bool gettingList READ gettingList NOTIFY gettingListChanged)
public:
    explicit AnimeList(QObject *parent = nullptr);

    Q_INVOKABLE static QStringList categoryList();

    int categoryIndex() const;
    void setCategoryIndex(int categoryIndex);
    const QString &category() const;

    AnimeModel *model() const;

    bool gettingList() const;

signals:
    void categoryIndexChanged(int categoryIndex);

    void gettingListChanged(bool gettingList);

public slots:

private slots:
    void requestFinished(QNetworkReply *);

private:
    AnimeModel *m_model;
    QNetworkAccessManager *m_networkManager;
    bool m_gettingList = false;
    int m_categoryIndex = 0;
};

#endif // ANIMELIST_HPP
