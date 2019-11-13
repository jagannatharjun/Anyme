#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

class AnimeList;

class Application : public QGuiApplication {
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);

    Q_INVOKABLE void loadAnimeInfo(int malId);

private:
    QQmlApplicationEngine m_engine;
    AnimeList *m_animeList;

    void load(const QString &path, const QVector<QQmlContext::PropertyPair> &props = {});
    void setContext(QQmlContext *ctx);
};

#endif // APPLICATION_HPP
