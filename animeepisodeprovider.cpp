#include "animeepisodeprovider.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQuickImageProvider>

class AnimePluginImageProvider : public QQuickImageProvider {
public:
    AnimePluginImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override {
        auto &pix = m_images[id];
        if (size)
            *size = pix.size();
        return pix.scaled(requestedSize);
    }

    void addImage(const QString &key, QPixmap map) { m_images[key] = std::move(map); }

private:
    QHash<QString, QPixmap> m_images;
};

AnimeEpisodeProvider::AnimeEpisodeProvider(QString name, QString scrapperPath, QObject *parent)
    : AnimeRequest(parent), m_name(std::move(name)), m_requestManager(scrapperPath, this),
      m_imageProvider(new AnimePluginImageProvider()) {
    connect(&m_requestManager, &EpisodeRequestManager::requestCompleted, this,
            &AnimeEpisodeProvider::requestCompleted);
}

void AnimeEpisodeProvider::search(const QString &keyWord) { setStatus(RequestStatus::InProgress); }

QString AnimeEpisodeProvider::imageProviderId() const { return m_name; }

EpisodesResults AnimeEpisodeProvider::episodeResults() const { return m_episodeResults; }

SearchResults AnimeEpisodeProvider::searchResults() const { return m_searchResults; }

void AnimeEpisodeProvider::searchEpisode(const QString &url) {
    setStatus(InProgress);
    m_requestManager.request(EpisodeRequestManager::Episodes, url);
}

QString AnimeEpisodeProvider::addImage(const QString &id, const QByteArray &imageData) {
    auto url = imageProviderId() + "://" + id;
    m_imageProvider->addImage(id, QPixmap::fromImage(QImage::fromData(imageData)));
    return url;
}

void AnimeEpisodeProvider::requestCompleted(EpisodeRequestManager::Requests r, QVariant res) {
    if (r == EpisodeRequestManager::Search) {
        Q_ASSERT(res.canConvert<SearchResults>());

        auto animes = res.value<SearchResults>();
        for (auto &a : animes) {
            a.setImageUrl(addImage(a.title(), a.imageData()));
        }
        m_searchResults = animes;
    } else if (r == EpisodeRequestManager::Episodes) {
        Q_ASSERT(res.canConvert<EpisodesResults>());
        m_episodeResults = res.value<EpisodesResults>();
    } else if (r == EpisodeRequestManager::Error) {
        setStatus(RequestStatus::Error, res.toString());
        return;
    }

    setStatus(RequestStatus::Completed);
}

EpisodeRequestManager::EpisodeRequestManager(QString program, QObject *parent)
    : QProcess(parent), m_currentRequest{Invalid} {
    setProgram("python");
    setArguments({"-u", program});
    connect(this, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this](int exitCode, QProcess::ExitStatus exitStatus) {
                qDebug("finished");
                qDebug() << exitCode << exitStatus;
            });
    connect(this, &QProcess::errorOccurred,
            [this](QProcess::ProcessError e) { qDebug() << errorString(); });
    connect(this, &QProcess::readyReadStandardOutput, this,
            &EpisodeRequestManager::readProcessOutput);
}

void EpisodeRequestManager::start() { QProcess::start(QProcess::Unbuffered | QProcess::ReadWrite); }

void EpisodeRequestManager::request(const EpisodeRequestManager::Requests type,
                                    const QString &argument) {

    Q_ASSERT(m_currentRequest == Invalid);
    Q_ASSERT(state() == QProcess::Running);

    QString cmd;
    switch (type) {
    case Search:
        cmd = "search " + argument;
        break;
    case Episodes:
        cmd = "episodes " + argument;
        break;
    case Error:
    default:
        Q_ASSERT(false && "Invalid Request");
    }

    m_currentRequest = type;
    qDebug() << write((cmd + '\n').toUtf8());
}

void EpisodeRequestManager::parseReply(const QByteArray &data) try {
    QVariant res;

    if (m_currentRequest == Search) {

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError)
            throw tr("Json Error in results of Search - %1").arg(err.errorString());
        if (!doc.isArray())
            throw tr("Search Json is not array");

        SearchResults animeList;
        auto arr = doc.array();
        for (auto val : arr) {
            auto obj = val.toObject();

            animeList.push_back(PluginAnime(
                obj["title"].toString(), obj["synopsis"].toString(), obj["url"].toString(),
                QByteArray::fromBase64(obj["image"].toString().toUtf8())));
        }

        res = QVariant::fromValue(animeList);
    } else if (m_currentRequest == Episodes) {

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError)
            throw tr("Json Error in results of Search - %1").arg(err.errorString());
        if (!doc.isArray())
            throw tr("Episode Json is not array");
        auto arr = doc.array();

        EpisodesResults eps;
        for (auto val : arr) {
            auto obj = val.toObject();

            QVector<EpisodeUrl> urls;
            for (auto jsonUrls : obj["urls"].toArray()) {
                urls.push_back(EpisodeUrl(jsonUrls.toObject()["url"].toString(),
                                          jsonUrls.toObject()["title"].toString()));
            }

            eps.push_back(Episode(urls, obj["title"].toString(), obj["title"].toString()));
        }

        res = QVariant::fromValue(eps);
    }

    requestCompleted(m_currentRequest, res);
} catch (const QString &err) {
    emit requestCompleted(Requests::Error, err);
}

void EpisodeRequestManager::readProcessOutput() {
    static QByteArray reply;
    static const QByteArray endSuffix = "$$$$$END$$$$$";
    static const QByteArray successPrefix = "result - ", errorPrefix = "error - ";

    reply.append(readAllStandardOutput());
    if (reply.endsWith(endSuffix)) {
        reply.chop(endSuffix.length());

        if (reply.startsWith(successPrefix)) {
            reply = reply.mid(successPrefix.length());
            parseReply(reply);
        } else if (reply.startsWith(errorPrefix)) {
            emit requestCompleted(Requests::Error, reply.mid(errorPrefix.length()));
        }

        m_currentRequest = Invalid;
        reply.clear();
    }

    if (state() != QProcess::Running && m_currentRequest != Invalid) {
        requestCompleted(Requests::Error, tr("Python Error - %1").arg(QString(reply)));

        m_currentRequest = Invalid;
        reply.clear();
    }
}

void PluginAnime::setImageUrl(const QString &imageUrl) { m_imageUrl = imageUrl; }
