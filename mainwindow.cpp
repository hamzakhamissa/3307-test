// #include "MainWindow.h"
// #include <QDebug>

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent)
// {
//     // Create a fixed-size window for simplicity
//     setFixedSize(800, 600);

//     // Create the character and set its parent to this window
//     character = new Character(this);

//     // Optional: set background color
//     setStyleSheet("background-color: lightblue;");
// }

// MainWindow::~MainWindow()
// {
//     delete character;
// }

// void MainWindow::keyPressEvent(QKeyEvent *event)
// {
//     switch (event->key()) {
//     case Qt::Key_W:
//         character->moveUp();
//         break;
//     case Qt::Key_S:
//         character->moveDown();
//         break;
//     case Qt::Key_A:
//         character->moveLeft();
//         break;
//     case Qt::Key_D:
//         character->moveRight();
//         break;
//     default:
//         QMainWindow::keyPressEvent(event);
//         break;
//     }
// }
