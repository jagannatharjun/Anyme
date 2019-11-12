#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickView>

#include <QFileSystemWatcher>

#include "animelist.hpp"

#define LIVE_QML

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");

    AnimeList a;
    a.setCategoryIndex(1);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/MainWindow.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

#ifdef LIVE_QML
    QQuickView v;

    QQmlContext *ctx = v.rootContext();
    ctx->setContextProperty("animelist", &a);

    QFileSystemWatcher f;
    f.addPath("E:/Cpp/Projects/Gui/anime/mainWindow.qml");
    f.connect(&f, &QFileSystemWatcher::fileChanged, [&](const QString &p) {
        v.setSource(QUrl());
        v.engine()->clearComponentCache();
        v.setSource(QUrl("file:///E:/Cpp/Projects/Gui/anime/mainWindow.qml"));
        f.addPath(p);
    });

    v.setSource(QUrl("file:///E:/Cpp/Projects/Gui/anime/mainWindow.qml"));
    v.resize(800,400);
    v.show();
#else

    QQmlContext *ctx = engine.rootContext();
    ctx->setContextProperty("animelist", &a);
    engine.load(QUrl("qrc:/MainWindow.qml"));
#endif

    return app.exec();
}
