#include <QApplication>
#include "game.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Game game;
    game.start();

    return app.exec();
}
