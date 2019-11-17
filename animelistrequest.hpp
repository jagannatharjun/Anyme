#ifndef MYANIMELIST_HPP
#define MYANIMELIST_HPP

#include <QObject>

#include "anime.hpp"
#include "animerequest.hpp"

class QNetworkAccessManager;
class QNetworkReply;
class QJsonDocument;
class QJsonArray;

class AnimeListRequest : public AnimeRequest {
    Q_OBJECT
public:
    explicit AnimeListRequest(QNetworkAccessManager *i, QObject *parent = nullptr);

private slots:
    void parseNetworkReply(QNetworkReply *r) override;

protected:
    virtual void addAnime(Anime anime) = 0;
    virtual void
    parseJsonReply(const QJsonDocument &doc) noexcept(false) = 0; // throw QString on error

    void request(const QUrl &url);
    void cancelCurrentRequest();
    void jsonArrayToAnimes(const QJsonArray &); // calls addAnime

private:
    QNetworkAccessManager *m_networkManager = nullptr;
    QNetworkReply *m_currentRequest = nullptr;
};

#endif // MYANIMELIST_HPP
