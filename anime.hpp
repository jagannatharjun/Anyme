#include <utility>

#include <utility>

#include <utility>

#ifndef ANIME_HPP
#define ANIME_HPP

#include <QDate>
#include <QObject>
#include <QString>
#include <QUrl>

class Anime {
    Q_GADGET
    Q_PROPERTY(int malId READ malId WRITE setMalId)
    Q_PROPERTY(int rank READ rank WRITE setRank)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(QUrl imageUrl READ imageUrl WRITE setImageurl)
    Q_PROPERTY(QString type READ type WRITE setType)
    Q_PROPERTY(int episodes READ episodes WRITE setEpisodes)
    Q_PROPERTY(QDate startDate READ startDate WRITE setStartdate)
    Q_PROPERTY(QDate endDate READ endDate WRITE setEnddate)
    Q_PROPERTY(int members READ members WRITE setMembers)
    Q_PROPERTY(double score READ score WRITE setScore)

public:

    int malId() const;

    int rand() const;

    int rank() const;

    QString title() const;

    QUrl url() const;

    QUrl imageUrl() const;

    QString type() const;

    int episodes() const;

    QDate startDate() const;

    QDate endDate() const;

    int members() const;

    double score() const;

public slots:
    void setMalId(int malId);

    void setRank(int rank);

    void setTitle(QString title);

    void setUrl(QUrl url);

    void setImageurl(QUrl imageUrl);

    void setType(QString type);

    void setEpisodes(int episodes);

    void setStartdate(QDate startDate);

    void setEnddate(QDate endDate);

    void setMembers(int members);

    void setScore(double score);

private:
    int m_malId;
    int m_rank;
    QString m_title;
    QUrl m_url;
    QUrl m_imageUrl;
    QString m_type;
    int m_episodes;
    QDate m_startDate;
    QDate m_endDate;
    int m_members;
    double m_score;
};

#endif // ANIME_HPP
