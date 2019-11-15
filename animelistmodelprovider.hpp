#ifndef ANIMELIST_HPP
#define ANIMELIST_HPP

#include <QAbstractItemModel>
#include <QObject>

#include "anime.hpp"

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
    Anime getAnimeById(int id) const;
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

class AnimeListModel;

class AnimeListModelProvider : public QObject {
    Q_OBJECT
    Q_PROPERTY(AnimeListModel *model READ model)
    Q_PROPERTY(
        int categoryIndex READ categoryIndex WRITE setCategoryIndex NOTIFY categoryIndexChanged)
    Q_PROPERTY(bool gettingList READ gettingList WRITE setGettingList NOTIFY gettingListChanged)
    Q_PROPERTY(bool isError READ isError NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorChanged)
public:
    explicit AnimeListModelProvider(QObject *parent = nullptr);

    Q_INVOKABLE static QStringList categoryList();

    int categoryIndex() const;
    void setCategoryIndex(int categoryIndex);
    const QString &category() const;

    Q_INVOKABLE void nextPage();
    AnimeListModel *model() const;
    bool gettingList() const;

    bool isError() const;

    QString errorString() const;

signals:
    void categoryIndexChanged(int categoryIndex);
    void gettingListChanged(bool gettingList);
    void errorChanged(bool isError);

private:
    AnimeListModel *m_model;
    AnimeListProvider *m_animeListProvider;
    bool m_gettingList = false;
    int m_categoryIndex = 0;
    int m_pageNumber = 1;

    void setGettingList(bool gettingList);
    void setIsError(bool isError, const QString& err);
    bool m_isError;
    QString m_errorString;
};

#endif // ANIMELIST_HPP
