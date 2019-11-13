#ifndef MYANIMELIST_HPP
#define MYANIMELIST_HPP

#include <QObject>

#include "anime.hpp"

class QNetworkAccessManager;
class QNetworkReply;

class AnimeListProvider : public QObject {
    Q_OBJECT
public:
    explicit AnimeListProvider(QObject *parent = nullptr);
    Q_INVOKABLE static const QStringList &categoryList();

signals:
    void gotAnime(Anime anime);
    void animeListRequestDone(bool failure, const QString& err);

public slots:
    void requestAnimeList(int page, int categoryIndex);

private slots:
    void parseAnimeList(QNetworkReply *r);

private:
    QNetworkAccessManager *m_networkManager;
};

#endif // MYANIMELIST_HPP
