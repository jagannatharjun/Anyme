#include "application.hpp"
#include "animelist.hpp"

#include <QQmlContext>

#define LIVE_QML

#ifdef LIVE_QML
#include <QFileSystemWatcher>
#include <QQuickView>
#endif

Application::Application(int &argc, char **argv)
    : QGuiApplication(argc, argv), m_animeList(new AnimeList(this)) {

    setContext(m_engine.rootContext());
    liveLoad(R"(E:/Cpp/Projects/Gui/anime/mainWindow.qml)");
}

void Application::liveLoad(const QString &path) {
#ifdef LIVE_QML
    auto view = new QQuickView();
    auto urlWatcher = new QFileSystemWatcher(this);
    urlWatcher->addPath(path);
    setContext(view->rootContext());
    view->setSource(QUrl::fromLocalFile(path));
    connect(urlWatcher, &QFileSystemWatcher::fileChanged,
            [view, urlWatcher](const QString &p) {
                view->setSource(QUrl());
                view->engine()->clearComponentCache();
                view->setSource(QUrl::fromLocalFile(p));
                urlWatcher->addPath(p);
            });
    view->show();
#endif
}

void Application::setContext(QQmlContext *ctx) {
    ctx->setContextProperty("application", this);
    ctx->setContextProperty("animelist", m_animeList);
}
