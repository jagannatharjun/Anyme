#include "application.hpp"
#include "animelist.hpp"

#include <QQmlContext>

#define LIVE_QML

#ifdef LIVE_QML
#include <QFileSystemWatcher>
#include <QQuickView>
#define MYQMLPATH(FILE) "E:/Cpp/Projects/Gui/anime/" FILE
#endif

Application::Application(int &argc, char **argv)
    : QGuiApplication(argc, argv), m_animeList(new AnimeList(this)) {

    setContext(m_engine.rootContext());

    load(MYQMLPATH("mainWindow.qml"));
}

void Application::loadAnimeInfo(int malId) {
    load(MYQMLPATH("anime.qml"),
         {{"anime", QVariant::fromValue(m_animeList->model()->getAnimeById(malId))}});
}

void Application::load(const QString &path, const QVector<QQmlContext::PropertyPair> &props) {
#ifdef LIVE_QML
    auto view = new QQuickView();
    auto urlWatcher = new QFileSystemWatcher(this);
    urlWatcher->addPath(path);
    view->rootContext()->setContextProperties(props);
    setContext(view->rootContext());
    view->setSource(QUrl::fromLocalFile(path));
    connect(urlWatcher, &QFileSystemWatcher::fileChanged, [view, urlWatcher](const QString &p) {
        view->setSource(QUrl());
        view->engine()->clearComponentCache();
        view->setSource(QUrl::fromLocalFile(p));
        urlWatcher->addPath(p);
    });
    view->resize(800,600);
    view->show();
#else
    static_assert(0, "lol");
#endif
}

void Application::setContext(QQmlContext *ctx) {
    ctx->setContextProperty("application", this);
    ctx->setContextProperty("animelist", m_animeList);
}
