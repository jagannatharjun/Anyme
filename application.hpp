#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QVector>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlPropertyMap>

class QNetworkAccessManager;
class AnimeListModelProvider;
class AnimeDetailsProvider;
class AnimeSearchProvider;

class Application : public QGuiApplication {
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);

    Q_INVOKABLE void loadAnimeInfo(int malId);

private:
    class Theme : public QQmlPropertyMap {
    public:
        Theme(QObject *parent = nullptr);
    };

    QQmlApplicationEngine m_engine;
    Theme m_theme;
    QNetworkAccessManager *m_networkManager;
    AnimeListModelProvider *m_animeList;
    AnimeDetailsProvider *m_animeDetailsProvider;
    AnimeSearchProvider *m_animeSearchProvider;

    QObject *load(const QString &path, const QVector<QQmlContext::PropertyPair> &props =  QVector<QQmlContext::PropertyPair> {});
    void setContext(QQmlContext *ctx);
};

#endif // APPLICATION_HPP
