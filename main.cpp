#include <QApplication>
// #include "MainWindow.h"
#include "ui/game.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Game game;
    game.start();

    // MainWindow w;
    // w.show();

    return app.exec();
}
