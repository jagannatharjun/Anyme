#include "application.hpp"

#include "animeepisodeprovider.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    Application app(argc, argv);

    EpisodeRequestManager manager("E:/Cpp/Projects/Gui/anime/scrapping/kissanime.py", nullptr);
    manager.start();
    manager.request(EpisodeRequestManager::Search, "My hero");
    return app.exec();
}
