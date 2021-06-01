#include "application.hpp"
#include "animedetailsprovider.hpp"
#include "animelistmodelprovider.hpp"
#include "animesearchprovider.hpp"

#include <QDebug>
#include <QDir>
#include <QNetworkAccessManager>
#include <QProcess>
#include <QQmlContext>
#include <QQuickStyle>


#ifdef DEFINE_STD_REVERSE

static inline void _Reverse_tail(unsigned long long * _First, unsigned long long * _Last) throw() {
  for (; _First != _Last && _First != --_Last; ++_First) {
    const auto _Temp = *_First;
    *_First = *_Last;
    *_Last = _Temp;
  }
}
extern "C"  __declspec(noalias) void __cdecl __std_reverse_trivially_swappable_8(void * _First, void * _Last) noexcept {
  _Reverse_tail(static_cast<unsigned long long *>(_First), static_cast<unsigned long long *>(_Last));
}

extern "C"  __declspec(noalias) void __cdecl __std_reverse_trivially_swappable_4(void * _First, void * _Last) noexcept {
  _Reverse_tail(static_cast<unsigned long long *>(_First), static_cast<unsigned long long *>(_Last));
}

#endif

//#define LIVE_QML

#ifdef LIVE_QML
#include <QFileSystemWatcher>
#define QMLDIR "E:/Cpp/Projects/Gui/anime/qml/"
#define MYQMLPATH(FILE) QMLDIR FILE
#define MYQMLURL(FILE) QUrl::fromLocalFile(MYQMLPATH(FILE))
static QFileSystemWatcher *qmlWatcher;

#else
#define MYQMLPATH(FILE) "qrc:///qml/" FILE	
#define MYQMLURL(FILE) QUrl(MYQMLPATH(FILE))
#endif

#include <QQuickView>

class MyQuickView : public QQuickView {
protected:
    bool event(QEvent *event) override {
        if (event->type() == QEvent::Close) {
            deleteLater();
        }
        return QQuickView::event(event);
    }
};

Application::Application(int &argc, char **argv)
    : QGuiApplication(argc, argv), m_networkManager(new QNetworkAccessManager(this)),
      m_animeList(new AnimeListModelProvider(m_networkManager, this)),
      m_animeDetailsProvider(new AnimeDetailsProvider(m_networkManager, this)),
      m_animeSearchProvider(new AnimeSearchProvider(m_networkManager, this)) {

    if (!QDir("anime-dl").exists()) {
    }

    QQuickStyle::setStyle("Material");
    setContext(m_engine.rootContext());
    //load(MYQMLPATH("mainWindow.qml"));
	load("qrc:///qml/MainWindow.qml");
}

void Application::loadAnimeInfo(int malId) {
    qDebug() << malId;
    auto rq = m_animeDetailsProvider->requestAnimeDetails(malId);
    auto p = load(MYQMLPATH("AnimeDetailsLoader.qml"),
                  {{"animeReq", QVariant::fromValue(rq)},
                   {"anime", QVariant::fromValue(rq->details())},
                   {"animeDetailsQmlSource", MYQMLURL("AnimeDetails.qml")}});
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
    });
    view->resize(936, 620);
    view->show();
    return view;
#else
    auto view = new MyQuickView();
    view->rootContext()->setContextProperties(props);
    setContext(view->rootContext());
    view->setSource(path);
    view->resize(936, 620);
    view->show();
    return view;	
#endif
}

void Application::setContext(QQmlContext *ctx) {
    ctx->setContextProperty("application", this);
    ctx->setContextProperty("animelist", m_animeList);
    ctx->setContextProperty("animesearch", m_animeSearchProvider);
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

    //        insert("accent", QColor(Qt::white));
    //        insert("accentForeground", QColor(Qt::black));

    //        insert("primary", QColor(Qt::white));
    //        insert("primaryDark", QColor(Qt::white));
    //        insert("primaryForeground", QColor(Qt::black));
    //        insert("primaryDarkForeground", QColor(Qt::black));

    //        insert("background", QColor(Qt::white));
    //        insert("foreground", QColor(Qt::black));
    //        insert("disabledText", QColor(96, 96, 96));
}
