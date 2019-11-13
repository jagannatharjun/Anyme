#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

class AnimeListModelProvider;
class AnimeDetailsProvider;

class Application : public QGuiApplication {
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);

    Q_INVOKABLE void loadAnimeInfo(int malId);

private:
    QQmlApplicationEngine m_engine;
    AnimeListModelProvider *m_animeList;
    AnimeDetailsProvider *m_animeDetailsProvider;

    QQmlContext *load(const QString &path, const QVector<QQmlContext::PropertyPair> &props = {});
    void setContext(QQmlContext *ctx);
};

#endif // APPLICATION_HPP
