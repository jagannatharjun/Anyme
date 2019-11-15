#ifndef MYANIMELIST_HPP
#define MYANIMELIST_HPP

#include <QObject>

#include "anime.hpp"
#include "animerequest.hpp"

class QNetworkAccessManager;
class QNetworkReply;

class AnimeListProvider : public AnimeRequest {
    Q_OBJECT
public:
    explicit AnimeListProvider(QNetworkAccessManager *i, QObject *parent = nullptr);
    Q_INVOKABLE static const QStringList &categoryList();

private slots:
    void parseNetworkReply(QNetworkReply *r) override;

protected:
    virtual void addAnime(Anime anime) = 0;

    void requestAnimeList(int page, int categoryIndex);
    void cancelCurrentRequest();

private:
    QNetworkAccessManager *m_networkManager = nullptr;
    QNetworkReply *m_currentRequest = nullptr;
};

#endif // MYANIMELIST_HPP
