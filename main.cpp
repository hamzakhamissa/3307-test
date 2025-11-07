#include <QApplication>
#include "game.h"
#include <iostream>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //Game game;
    //game.start();

    MainWindow w;
    w.show();

    return app.exec();
}
