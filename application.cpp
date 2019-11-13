#include "application.hpp"
#include "animelistmodelprovider.hpp"

#include <QDebug>
#include <QQmlContext>

#define LIVE_QML

#ifdef LIVE_QML
#include <QFileSystemWatcher>
#include <QQuickView>
#define QMLDIR "E:/Cpp/Projects/Gui/anime/qml/"
#define MYQMLPATH(FILE) QMLDIR FILE
static QFileSystemWatcher *qmlWatcher;
#endif

Application::Application(int &argc, char **argv)
    : QGuiApplication(argc, argv), m_animeList(new AnimeListModelProvider(this)) {

    setContext(m_engine.rootContext());

    load(MYQMLPATH("mainWindow.qml"));
}

void Application::loadAnimeInfo(int malId) {
    qDebug() << malId;
    load(MYQMLPATH("anime.qml"),
         {{"anime", QVariant::fromValue(m_animeList->model()->getAnimeById(malId))}});
}

void Application::load(const QString &path, const QVector<QQmlContext::PropertyPair> &props) {
#ifdef LIVE_QML
    auto view = new QQuickView();
    if (!qmlWatcher) {
        qmlWatcher = new QFileSystemWatcher(this);
        qmlWatcher->addPath(QMLDIR);
    }
    view->rootContext()->setContextProperties(props);
    setContext(view->rootContext());
    view->setSource(QUrl::fromLocalFile(path));
    connect(qmlWatcher, &QFileSystemWatcher::directoryChanged, [view, path](const QString &) {
        view->setSource(QUrl());
        view->engine()->clearComponentCache();
        view->setSource(QUrl::fromLocalFile(path));
        qmlWatcher->addPath(QMLDIR);
    });
    view->resize(800, 600);
    view->show();
#else
    static_assert(0, "lol");
#endif
}

void Application::setContext(QQmlContext *ctx) {
    ctx->setContextProperty("application", this);
    ctx->setContextProperty("animelist", m_animeList);
}
