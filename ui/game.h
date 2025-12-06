#ifndef GAME_H
#define GAME_H

#include <QElapsedTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QWidget>
#include <map>
#include <memory>

#include "player.h"
#include "tileboard.h"
#include "market.h"
#include "menu.h"

enum class ItemTypeEnum;

class Game : public QWidget
{
    Q_OBJECT

public:
    Game(QWidget *parent = nullptr);
    ~Game();

    bool init();
    void start();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void gameLoop();

private:
    QTimer *gameTimer;
    bool running;
    bool shopOpen;
    bool menuOpen;
    bool shopBuyMode; // true = buy mode, false = sell mode
    int dayNumber;

    // Constants
    static const int WINDOW_WIDTH  = 1024;
    static const int WINDOW_HEIGHT = 768;
    static const int TILE_SIZE     = 32;
    static const int CHARACTER_SIZE = 128;

    // Systems
    std::unique_ptr<Player> player;
    TileBoard tiles;
    Market market;
    Menu menu;

    // Camera
    float cameraX;
    float cameraY;

    // Character & item textures (tile textures are in TileBoard)
    QPixmap charDownTex, charLeftTex, charRightTex, charUpTex, charIdleTex;
    QPixmap hoeTex, seedTex, wateringCanTex;
    QPixmap wheatSeedTex, tomatoSeedTex, cornSeedTex;
    QPixmap wheatTex, tomatoTex, cornTex;
    QPixmap milkTex, eggTex;
    QPixmap cowTex, chickenTex;

    // Input
    void handleInput();
    std::map<int, bool> keys;
    bool mouseLeftPressed;
    bool mouseLeftPressedLastFrame;

    // Interaction helpers
    void interactWithFrontTile();
    void getTileAtPlayer(int &tileX, int &tileY);

    // Loop
    void update(float deltaTime);
    void render(QPainter &painter);

    // Timing
    QElapsedTimer *elapsedTimer;

    // Growth
    void advanceGrowthCycle();
    
    // Helper functions
    void openShop();
    void closeShop();
    void openMenu();
    void closeMenu();
    void saveGame(int saveSlot = 1);
    void loadGame(int saveSlot = 1);
    
    // UI Rendering
    void renderInventoryUI(QPainter& painter);
    void renderShopUI(QPainter& painter);
    void renderMenuUI(QPainter& painter);
    const QPixmap* getItemSprite(ItemTypeEnum itemType) const;
    void renderTileIndicator(QPainter& painter);
};

#endif
