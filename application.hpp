#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlPropertyMap>

class AnimeListModelProvider;
class AnimeDetailsProvider;

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
    AnimeListModelProvider *m_animeList;
    AnimeDetailsProvider *m_animeDetailsProvider;

    QQmlContext *load(const QString &path, const QVector<QQmlContext::PropertyPair> &props = {});
    void setContext(QQmlContext *ctx);
};

#endif // APPLICATION_HPP
