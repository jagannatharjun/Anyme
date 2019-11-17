#include "application.hpp"
#include "animedetailsprovider.hpp"
#include "animelistmodelprovider.hpp"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QQmlContext>
#include <QQuickStyle>

#define LIVE_QML

#ifdef LIVE_QML
#include <QFileSystemWatcher>
#include <QQuickView>
#define QMLDIR "E:/Cpp/Projects/Gui/anime/qml/"
#define MYQMLPATH(FILE) QMLDIR FILE
#define MYQMLURL(FILE) QUrl::fromLocalFile(MYQMLPATH(FILE))
static QFileSystemWatcher *qmlWatcher;

class MyQuickView : public QQuickView {
protected:
    bool event(QEvent *event) override {
        if (event->type() == QEvent::Close) {
            deleteLater();
        }
        return QQuickView::event(event);
    }
};

#endif

Application::Application(int &argc, char **argv)
    : QGuiApplication(argc, argv), m_networkManager(new QNetworkAccessManager(this)),
      m_animeList(new AnimeListModelProvider(m_networkManager, this)),
      m_animeDetailsProvider(new AnimeDetailsProvider(m_networkManager, this)) {
    QQuickStyle::setStyle("Material");
    setContext(m_engine.rootContext());
    load(MYQMLPATH("mainWindow.qml"));
}

void Application::loadAnimeInfo(int malId) {
    qDebug() << malId;
    auto rq = m_animeDetailsProvider->requestAnimeDetails(malId);
    auto p = load(MYQMLPATH("animeDetailsLoader.qml"),
                  {{"animeReq", QVariant::fromValue(rq)},
                   {"anime", QVariant::fromValue(rq->details())},
                   {"animeDetailsQmlSource", MYQMLURL("animeDetails.qml")}});
    rq->setParent(p);
}

QObject *Application::load(const QString &path, const QVector<QQmlContext::PropertyPair> &props) {
#ifdef LIVE_QML
    auto view = new MyQuickView();
    if (!qmlWatcher) {
        qmlWatcher = new QFileSystemWatcher(this);
        qmlWatcher->addPath(QMLDIR);
    }
    view->rootContext()->setContextProperties(props);
    setContext(view->rootContext());
    view->setSource(QUrl::fromLocalFile(path));
    connect(qmlWatcher, &QFileSystemWatcher::directoryChanged, [view, path](const QString &p) {
        view->setSource(QUrl());
        view->engine()->clearComponentCache();
        view->setSource(QUrl::fromLocalFile(path));
        qmlWatcher->addPath(QMLDIR);
    });
    view->resize(900, 620);
    view->show();
    return view;
#else
    static_assert(0, "lol");
#endif
}

void Application::setContext(QQmlContext *ctx) {
    ctx->setContextProperty("application", this);
    ctx->setContextProperty("animelist", m_animeList);
    ctx->setContextProperty("theme", &m_theme);
}

Application::Theme::Theme(QObject *parent) : QQmlPropertyMap(parent) {
    insert("primary", QColor("#F8BBD0"));
    insert("primaryDark", QColor("#C2185B"));
    insert("primaryDarkForeground", QColor(Qt::white));
    insert("primaryForeground", QColor(61, 61, 61));

    insert("accent", QColor("#448AFF"));
    insert("accentForeground", QColor(Qt::white));

    insert("background", QColor("#303030"));
    insert("foreground", QColor(Qt::white));

    insert("disabledText", QColor(96, 96, 96));

    //    insert("accent", QColor());

    //    insert("primary", QColor(Qt::white));
    //    insert("primaryDark", QColor(Qt::white));
    //    insert("primaryForeground", QColor(Qt::black));
    //    insert("primarySurface", QColor(Qt::black));
    //    insert("primarySurfaceForegound", QColor(Qt::black));

    //    insert("background", QColor(Qt::white));
    //    insert("foreground", QColor(Qt::black));
}
