#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QGuiApplication>
#include <QQmlApplicationEngine>

class AnimeList;

class Application : public QGuiApplication {
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);

private:
    QQmlApplicationEngine m_engine;
    AnimeList *m_animeList;

    void liveLoad(const QString &path);
    void setContext(QQmlContext *ctx);
};

#endif // APPLICATION_HPP
