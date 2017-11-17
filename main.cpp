#include <QApplication>
#include <QDesktopWidget>
#include "game.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("fusion");
    auto desktop = QApplication::desktop()->geometry();
    Game game;
    auto dx = (desktop.width() - game.width()) / 2;
    auto dy = (desktop.height() - game.height()) / 2;
    game.move(dx, dy);
    game.run();
    return app.exec();
}
