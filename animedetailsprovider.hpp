#ifndef ANIMEDETAILSPROVIDER_HPP
#define ANIMEDETAILSPROVIDER_HPP

#include <QObject>

#include <QQmlPropertyMap>

#include<QDebug>

class AnimeDetails : public QQmlPropertyMap {
    Q_OBJECT
public:
    AnimeDetails(QObject *parent = nullptr) : QQmlPropertyMap(parent) {}
    ~AnimeDetails() {
        qDebug(__PRETTY_FUNCTION__);
    }
};

class AnimeDetailsRequest : public QObject {
    Q_OBJECT
public:
    AnimeDetailsRequest(QObject *parent = nullptr)
        : QObject(parent), m_details(new AnimeDetails(this)) {}

    enum Status { InProgress, Completed, Failed };
    Status status() const { return m_status; }

    friend class AnimeDetailsProvider;
    AnimeDetails *details() const;

signals:
    void completed(AnimeDetails *m_details);
    void failed(const QString &err);

private:
    Status m_status;
    AnimeDetails *m_details;

    void setStatus(Status s, const QString &e = {}) {
        if (s == Completed) {
            emit completed(m_details);
        } else if (s == Failed) {
            emit failed(e);
        }
        m_status = s;
    }

    void setAnimeDetailsProp(const char *prop, const QVariant &v) {
        m_details->insert(prop, v);
    }
};

class AnimeDetailsProvider : public QObject {
    Q_OBJECT
public:
    AnimeDetailsProvider(QObject *parent);

    AnimeDetailsRequest *requestAnimeDetails(int malId);

private:
    class QNetworkAccessManager *m_networkManager;

private:
    static void parseJsonDetails(AnimeDetailsRequest *rq, const QByteArray &data);
};

#endif // ANIMEDETAILSPROVIDER_HPP
