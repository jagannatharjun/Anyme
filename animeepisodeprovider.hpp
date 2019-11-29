#ifndef ANIMEEPISODEPROVIDER_HPP
#define ANIMEEPISODEPROVIDER_HPP

#include "anime.hpp"
#include "animelistmodel.hpp"
#include "animerequest.hpp"

#include <QObject>
#include <QProcess>
#include <QQmlPropertyMap>

class AnimePluginImageProvider;
class QQuickImageProvier;
class PluginAnime;
class Episode;

using SearchResults = QVector<PluginAnime>;
using EpisodesResults = QVector<Episode>;

class EpisodeUrl {
    Q_GADGET
    Q_PROPERTY(QString url READ url CONSTANT)
    Q_PROPERTY(QString title READ title CONSTANT)

public:
    EpisodeUrl(QString url, QString title) : m_url(url), m_title(title) {}

    QString url() const { return m_url; }
    QString title() const { return m_title; }

private:
    QString m_url;
    QString m_title;
};

class Episode {
    Q_GADGET
    Q_PROPERTY(QVector<EpisodeUrl> urls READ urls CONSTANT)
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString date READ date CONSTANT)

public:
    Episode(QVector<EpisodeUrl> urls, QString title, QString date)
        : m_urls{urls}, m_title{title}, m_date{date} {}

    QVector<EpisodeUrl> urls() const { return m_urls; }
    QString title() const { return m_title; }
    QString date() const { return m_date; }

private:
    QVector<EpisodeUrl> m_urls;
    QString m_title;
    QString m_date;
};

class PluginAnime {
    Q_GADGET

    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString synopsis READ synopsis CONSTANT)
    Q_PROPERTY(QString url READ url CONSTANT)
    Q_PROPERTY(QString imageUrl READ imageUrl CONSTANT)

public:
    PluginAnime(QString title, QString synopsis, QString url, QByteArray imageData)
        : m_title{title}, m_synopsis{synopsis}, m_url{url}, m_imageData{imageData} {}

    QString title() const { return m_title; }
    QString synopsis() const { return m_synopsis; }
    QString url() const { return m_url; }
    QByteArray imageData() const { return m_imageData; }

    QString imageUrl() const { return m_imageUrl; }

    void setImageUrl(const QString &imageUrl);

private:
    QString m_title;
    QString m_synopsis;
    QString m_url;
    QByteArray m_imageData;
    QString m_imageUrl;
};

class EpisodeRequestManager : public QProcess {
    Q_OBJECT
public:
    enum Requests { Invalid, Error, Search, Episodes };

    EpisodeRequestManager(QString program, QObject *parent);

    void start();
    void request(Requests type, const QString &argument);

signals:
    void requestCompleted(Requests type, QVariant result);

private:
    Requests m_currentRequest;
    void parseReply(const QByteArray &data);

private slots:
    void readProcessOutput();
};

class AnimeEpisodeProvider : public AnimeRequest {
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
public:
    explicit AnimeEpisodeProvider(QString name, QString scrapperPath, QObject *parent = nullptr);

    QString name() const { return m_name; }

    Q_INVOKABLE void search(const QString &keyWord);
    Q_INVOKABLE SearchResults searchResults() const;

    Q_INVOKABLE void searchEpisode(const QString &url);
    Q_INVOKABLE EpisodesResults episodeResults() const;

    QString imageProviderId() const;
    QQuickImageProvier *imageProvider;

signals:
    void animeSearchCompleted(QVector<Anime> animes);
    void episodeSearchCompleted(QVector<Episode> eps);

private:
    QString m_name;
    EpisodeRequestManager m_requestManager;
    AnimePluginImageProvider *m_imageProvider;
    SearchResults m_searchResults;
    EpisodesResults m_episodeResults;

    QString addImage(const QString &id, const QByteArray &imageData);

private slots:
    void requestCompleted(EpisodeRequestManager::Requests r, QVariant res);
};

#endif // ANIMEEPISODEPROVIDER_HPP
