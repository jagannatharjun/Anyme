#ifndef ANIMELIST_HPP
#define ANIMELIST_HPP

#include "animelistrequest.hpp"
#include "animelistmodel.hpp"

class AnimeListModelProvider : public AnimeListRequest {
    Q_OBJECT
    Q_PROPERTY(AnimeListModel *model READ model)
    Q_PROPERTY(
        int categoryIndex READ categoryIndex WRITE setCategoryIndex NOTIFY categoryIndexChanged)

public:
    explicit AnimeListModelProvider(QNetworkAccessManager * i, QObject *parent = nullptr);

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
    void parseJsonReply(const QJsonDocument&) override;
    void requestAnimeList(int page, int categoryIndex);
};

#endif // ANIMELIST_HPP
