#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QString>
#include <QElapsedTimer>
#include <string>
#include <vector>
#include <map>

// Tile states
enum TileState {
    TILE_GRASS = 0,
    TILE_DIRT = 1,
    TILE_SEEDED = 2,
    TILE_PLANT_PHASE_1 = 3,
    TILE_PLANT_PHASE_2 = 4,
    TILE_PLANT_PHASE_3 = 5,
    TILE_HARVESTABLE = 6
};

// Player facing direction
enum PlayerDirection {
    DIR_DOWN = 0,
    DIR_LEFT = 1,
    DIR_RIGHT = 2,
    DIR_UP = 3
};

// Inventory items
enum ItemType {
    ITEM_NONE = 0,
    ITEM_HOE = 1,
    ITEM_SEED = 2
};

class Game : public QWidget {
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
    QTimer* gameTimer;
    bool running;

    // Game constants
    static const int WINDOW_WIDTH = 1024;
    static const int WINDOW_HEIGHT = 768;
    static const int TILE_SIZE = 32;
    static const int CHARACTER_SIZE = 128;


    float playerX;
    float playerY;
    float playerSpeed;
    PlayerDirection playerDirection;
    bool playerMoving;

    float cameraX;
    float cameraY;


    static const int MAP_WIDTH = 50;
    static const int MAP_HEIGHT = 50;

    // Tile map - stores tile states
    TileState tileMap[MAP_HEIGHT][MAP_WIDTH];

    // Inventory system
    ItemType hotbar[2];  // Slot 1 = hoe, Slot 2 = seed
    int selectedSlot;     // 0 = slot 1 (hoe), 1 = slot 2 (seed)

    // Sprites/Textures (using QPixmap instead of SDL_Texture)
    QPixmap charDownTex;
    QPixmap charLeftTex;
    QPixmap charRightTex;
    QPixmap charUpTex;
    QPixmap charIdleTex;
    QPixmap tileTex;      // Grass tile
    QPixmap dirtTex;     // Dirt tile
    QPixmap hoeTex;      // Hoe item
    QPixmap seedTex;     // Seed item
    QPixmap seed1Tex;    // Planted seed
    QPixmap plantPhase1Tex;
    QPixmap plantPhase2Tex;
    QPixmap plantPhase3Tex;

    // Texture management
    QPixmap loadTexture(const QString& path);
    bool loadAllTextures();
    void cleanupTextures();

    // Rendering
    void renderTile(QPainter& painter, int tileX, int tileY, int screenX, int screenY);
    void renderPlayer(QPainter& painter);
    void renderItemInHand(QPainter& painter);

    // Input handling
    void handleInput();
    std::map<int, bool> keys;  // Qt key codes
    bool mouseLeftPressed;
    bool mouseLeftPressedLastFrame;

    // Interaction
    void interactWithTile(int tileX, int tileY);
    void getTileAtPlayer(int& tileX, int& tileY);

    // Game loop
    void update(float deltaTime);
    void render(QPainter& painter);
    
    // Timing
    QElapsedTimer* elapsedTimer;

    // Crop growth
    void advanceGrowthCycle();
};

#endif // GAME_H
