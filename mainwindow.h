#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "Character.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Character* character;

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // MAINWINDOW_H
