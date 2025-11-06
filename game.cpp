#include "game.h"
#include <iostream>
#include <cmath>
#include <QElapsedTimer>

Game::Game(QWidget *parent) : QWidget(parent),
    gameTimer(nullptr), running(false),
    playerX(400.0f), playerY(300.0f), playerSpeed(100.0f),
    playerDirection(DIR_DOWN), playerMoving(false),
    cameraX(0.0f), cameraY(0.0f),
    selectedSlot(0), mouseLeftPressed(false), mouseLeftPressedLastFrame(false),
    elapsedTimer(nullptr) {
    // Initialize tile map with grass
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            tileMap[y][x] = TILE_GRASS;
        }
    }

    // Initialize inventory: Slot 1 = Hoe, Slot 2 = Seed
    hotbar[0] = ITEM_HOE;
    hotbar[1] = ITEM_SEED;
    
    // Set window properties
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowTitle("Farmland - Farming Simulator");
    
    // Enable keyboard focus
    setFocusPolicy(Qt::StrongFocus);
}

Game::~Game() {
}

bool Game::init() {
    // Load all textures
    if (!loadAllTextures()) {
        std::cerr << "Failed to load some textures!" << std::endl;
        return false;
    }

    // Create game timer for 60 FPS
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &Game::gameLoop);
    gameTimer->start(16); // ~60 FPS (1000ms / 60 ≈ 16ms)

    // Initialize timing
    elapsedTimer = new QElapsedTimer();
    elapsedTimer->start();
    
    running = true;
    return true;
}

void Game::start() {
    show();
    if (init()) {
        // Game loop is handled by QTimer
    }
}

void Game::gameLoop() {
    if (!running) {
        return;
    }

    // Calculate delta time
    static qint64 lastTime = -1;
    qint64 currentTime = elapsedTimer->elapsed();
    float deltaTime;
    
    if (lastTime < 0) {
        // First frame, assume 60 FPS
        deltaTime = 0.016f;
        lastTime = currentTime;
    } else {
        deltaTime = (currentTime - lastTime) / 1000.0f; // Convert to seconds
        lastTime = currentTime;
        
        // Cap delta time to prevent large jumps
        if (deltaTime > 0.1f) deltaTime = 0.1f;
    }

    handleInput();
    update(deltaTime);
    repaint(); // Trigger repaint
}

bool Game::loadAllTextures() {
    charDownTex.load(":/assets/assets/Down_character.png");
    charLeftTex.load(":/assets/assets/Left_character.png");
    charRightTex.load(":/assets/assets/Right_character.png");
    charUpTex.load(":/assets/assets/Up_character.png");
    charIdleTex.load(":/assets/assets/Idle_character.png");
    tileTex.load(":/assets/assets/tile.png");
    dirtTex.load(":/assets/assets/dirt.png");
    hoeTex.load(":/assets/assets/hoe.png");
    seedTex.load(":/assets/assets/Seed_1.png");
    seed1Tex.load(":/assets/assets/Seed_1.png");  // Same as seed for now
    plantPhase1Tex.load(":/assets/assets/Plant_1_Phase_1.png");
    plantPhase2Tex.load(":/assets/assets/Plant_1_Phase_2.png");
    plantPhase3Tex.load(":/assets/assets/Plant_1_Phase_3.png");

    // Check if all critical textures loaded
    if (charDownTex.isNull() || tileTex.isNull() || dirtTex.isNull() || 
        hoeTex.isNull() || seedTex.isNull()) {
        std::cerr << "Warning: Some critical textures failed to load!" << std::endl;
        return false;
    }

    return true;
}

void Game::cleanupTextures() {
    // QPixmap handles cleanup automatically, but we can clear them
    charDownTex = QPixmap();
    charLeftTex = QPixmap();
    charRightTex = QPixmap();
    charUpTex = QPixmap();
    charIdleTex = QPixmap();
    tileTex = QPixmap();
    dirtTex = QPixmap();
    hoeTex = QPixmap();
    seedTex = QPixmap();
    seed1Tex = QPixmap();
    plantPhase1Tex = QPixmap();
    plantPhase2Tex = QPixmap();
    plantPhase3Tex = QPixmap();
}

void Game::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Clear screen with sky blue background
    painter.fillRect(rect(), QColor(135, 206, 235));
    
    render(painter);
}

void Game::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    keys[key] = true;
    
    // ESC key to exit
    if (key == Qt::Key_Escape) {
        running = false;
        close();
    }
    // T key to advance growth cycle
    if (key == Qt::Key_T) {
        advanceGrowthCycle();
    }
    // Number keys for inventory slots
    if (key == Qt::Key_1) {
        selectedSlot = 0; // Hoe
    }
    if (key == Qt::Key_2) {
        selectedSlot = 1; // Seed
    }
    // E key for interaction
    if (key == Qt::Key_E) {
        int tileX, tileY;
        getTileAtPlayer(tileX, tileY);
        if (tileX >= 0 && tileX < MAP_WIDTH && tileY >= 0 && tileY < MAP_HEIGHT) {
            interactWithTile(tileX, tileY);
        }
    }
}

void Game::keyReleaseEvent(QKeyEvent *event) {
    keys[event->key()] = false;
}

void Game::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mouseLeftPressed = true;
        // Interact on click
        int tileX, tileY;
        getTileAtPlayer(tileX, tileY);
        if (tileX >= 0 && tileX < MAP_WIDTH && tileY >= 0 && tileY < MAP_HEIGHT) {
            interactWithTile(tileX, tileY);
        }
    }
}

void Game::handleInput() {
    mouseLeftPressedLastFrame = mouseLeftPressed;
    mouseLeftPressed = false;
    // Input is now handled by Qt event handlers
}

void Game::getTileAtPlayer(int& tileX, int& tileY) {
    // Get tile in front of player based on direction
    // Use CHARACTER_SIZE to get the actual center of the character sprite
    int centerTileX = (int)(playerX + CHARACTER_SIZE / 2) / TILE_SIZE;
    int centerTileY = (int)(playerY + CHARACTER_SIZE / 2) / TILE_SIZE;

    // Adjust based on facing direction - get tile directly in front of character
    switch (playerDirection) {
    case DIR_UP:
        tileX = centerTileX;
        tileY = centerTileY - 1;
        break;
    case DIR_DOWN:
        tileX = centerTileX;
        tileY = centerTileY + 1;
        break;
    case DIR_LEFT:
        tileX = centerTileX - 1;
        tileY = centerTileY;
        break;
    case DIR_RIGHT:
        tileX = centerTileX + 1;
        tileY = centerTileY;
        break;
    }
}

void Game::interactWithTile(int tileX, int tileY) {
    if (tileX < 0 || tileX >= MAP_WIDTH || tileY < 0 || tileY >= MAP_HEIGHT) {
        return;
    }

    TileState& tile = tileMap[tileY][tileX];
    ItemType selectedItem = hotbar[selectedSlot];

    // Hoe interaction - till grass into dirt
    if (selectedItem == ITEM_HOE && tile == TILE_GRASS) {
        tile = TILE_DIRT;
        std::cout << "Tilled ground at (" << tileX << ", " << tileY << ")" << std::endl;
    }
    // Seed interaction - plant seed on dirt
    else if (selectedItem == ITEM_SEED && tile == TILE_DIRT) {
        tile = TILE_SEEDED;
        std::cout << "Planted seed at (" << tileX << ", " << tileY << ")" << std::endl;
    }
    // Harvest interaction - harvest fully grown plant
    else if (tile == TILE_HARVESTABLE) {
        tile = TILE_DIRT; // Return to dirt after harvest
        std::cout << "Harvested crop at (" << tileX << ", " << tileY << ")" << std::endl;
    }
}

void Game::advanceGrowthCycle() {
    std::cout << "Advancing growth cycle..." << std::endl;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            TileState& tile = tileMap[y][x];
            // Advance growth stages
            if (tile == TILE_SEEDED) {
                tile = TILE_PLANT_PHASE_1;
            } else if (tile == TILE_PLANT_PHASE_1) {
                tile = TILE_PLANT_PHASE_2;
            } else if (tile == TILE_PLANT_PHASE_2) {
                tile = TILE_PLANT_PHASE_3;
            } else if (tile == TILE_PLANT_PHASE_3) {
                tile = TILE_HARVESTABLE;
            }
        }
    }
}

void Game::update(float deltaTime) {
    // Player movement with WASD
    float moveX = 0.0f, moveY = 0.0f;
    PlayerDirection newDirection = playerDirection;

    if (keys[Qt::Key_W] || keys[Qt::Key_Up]) {
        moveY -= 1.0f;
        newDirection = DIR_UP;
    }
    if (keys[Qt::Key_S] || keys[Qt::Key_Down]) {
        moveY += 1.0f;
        newDirection = DIR_DOWN;
    }
    if (keys[Qt::Key_A] || keys[Qt::Key_Left]) {
        moveX -= 1.0f;
        newDirection = DIR_LEFT;
    }
    if (keys[Qt::Key_D] || keys[Qt::Key_Right]) {
        moveX += 1.0f;
        newDirection = DIR_RIGHT;
    }

    playerMoving = (moveX != 0.0f || moveY != 0.0f);
    if (playerMoving) {
        playerDirection = newDirection;
    }

    // Normalize diagonal movement
    if (moveX != 0.0f && moveY != 0.0f) {
        moveX *= 0.707f; // 1/sqrt(2)
        moveY *= 0.707f;
    }

    // Update player position
    playerX += moveX * playerSpeed * deltaTime;
    playerY += moveY * playerSpeed * deltaTime;

    // Keep player within map bounds - account for CHARACTER_SIZE
    if (playerX < 0) playerX = 0;
    if (playerX > MAP_WIDTH * TILE_SIZE - CHARACTER_SIZE) playerX = MAP_WIDTH * TILE_SIZE - CHARACTER_SIZE;
    if (playerY < 0) playerY = 0;
    if (playerY > MAP_HEIGHT * TILE_SIZE - CHARACTER_SIZE) playerY = MAP_HEIGHT * TILE_SIZE - CHARACTER_SIZE;

    // Update camera to follow player (centered on character)
    cameraX = playerX + CHARACTER_SIZE / 2.0f - WINDOW_WIDTH / 2.0f;
    cameraY = playerY + CHARACTER_SIZE / 2.0f - WINDOW_HEIGHT / 2.0f;

    // Clamp camera to map bounds
    if (cameraX < 0) cameraX = 0;
    if (cameraX > MAP_WIDTH * TILE_SIZE - WINDOW_WIDTH) cameraX = MAP_WIDTH * TILE_SIZE - WINDOW_WIDTH;
    if (cameraY < 0) cameraY = 0;
    if (cameraY > MAP_HEIGHT * TILE_SIZE - WINDOW_HEIGHT) cameraY = MAP_HEIGHT * TILE_SIZE - WINDOW_HEIGHT;
}

void Game::render(QPainter& painter) {
    // Calculate which tiles to render (visible tiles only)
    int startTileX = (int)(cameraX / TILE_SIZE);
    int startTileY = (int)(cameraY / TILE_SIZE);
    int endTileX = startTileX + (WINDOW_WIDTH / TILE_SIZE) + 2;
    int endTileY = startTileY + (WINDOW_HEIGHT / TILE_SIZE) + 2;

    // Clamp to map bounds
    if (startTileX < 0) startTileX = 0;
    if (startTileY < 0) startTileY = 0;
    if (endTileX > MAP_WIDTH) endTileX = MAP_WIDTH;
    if (endTileY > MAP_HEIGHT) endTileY = MAP_HEIGHT;

    // Render tiles
    for (int y = startTileY; y < endTileY; y++) {
        for (int x = startTileX; x < endTileX; x++) {
            int screenX = x * TILE_SIZE - (int)cameraX;
            int screenY = y * TILE_SIZE - (int)cameraY;
            renderTile(painter, x, y, screenX, screenY);
        }
    }

    // Render player
    renderPlayer(painter);

    // Render item in hand
    renderItemInHand(painter);
}

void Game::renderTile(QPainter& painter, int tileX, int tileY, int screenX, int screenY) {
    QRect destRect(screenX, screenY, TILE_SIZE, TILE_SIZE);
    TileState tileState = tileMap[tileY][tileX];
    QPixmap* textureToRender = nullptr;

    switch (tileState) {
    case TILE_GRASS:
        textureToRender = &tileTex;
        break;
    case TILE_DIRT:
        textureToRender = &dirtTex;
        break;
    case TILE_SEEDED:
        // Render dirt with seed on top
        if (!dirtTex.isNull()) {
            painter.drawPixmap(destRect, dirtTex);
        }
        if (!seed1Tex.isNull()) {
            painter.drawPixmap(destRect, seed1Tex);
        }
        return; // Early return since we rendered both
    case TILE_PLANT_PHASE_1:
        if (!dirtTex.isNull()) {
            painter.drawPixmap(destRect, dirtTex);
        }
        if (!plantPhase1Tex.isNull()) {
            painter.drawPixmap(destRect, plantPhase1Tex);
        }
        return;
    case TILE_PLANT_PHASE_2:
        if (!dirtTex.isNull()) {
            painter.drawPixmap(destRect, dirtTex);
        }
        if (!plantPhase2Tex.isNull()) {
            painter.drawPixmap(destRect, plantPhase2Tex);
        }
        return;
    case TILE_PLANT_PHASE_3:
        if (!dirtTex.isNull()) {
            painter.drawPixmap(destRect, dirtTex);
        }
        if (!plantPhase3Tex.isNull()) {
            painter.drawPixmap(destRect, plantPhase3Tex);
        }
        return;
    case TILE_HARVESTABLE:
        if (!dirtTex.isNull()) {
            painter.drawPixmap(destRect, dirtTex);
        }
        if (!plantPhase3Tex.isNull()) {
            painter.drawPixmap(destRect, plantPhase3Tex);
        }
        return;
    }

    // Render base tile texture
    if (textureToRender && !textureToRender->isNull()) {
        painter.drawPixmap(destRect, *textureToRender);
    } else {
        // Fallback to colored rectangle if texture not loaded
        painter.fillRect(destRect, QColor(128, 128, 128));
    }
}

void Game::renderPlayer(QPainter& painter) {
    // Calculate player position on screen
    int screenX = (int)(playerX - cameraX);
    int screenY = (int)(playerY - cameraY);

    // Character sprite size
    QRect destRect(screenX, screenY, CHARACTER_SIZE, CHARACTER_SIZE);
    QPixmap* charTex = &charIdleTex; // Default to idle

    // Select character sprite based on direction and movement
    if (playerMoving) {
        switch (playerDirection) {
        case DIR_DOWN:
            charTex = &charDownTex;
            break;
        case DIR_LEFT:
            charTex = &charLeftTex;
            break;
        case DIR_RIGHT:
            charTex = &charRightTex;
            break;
        case DIR_UP:
            charTex = &charUpTex;
            break;
        }
    }

    // Fallback if texture not loaded
    if (charTex->isNull()) {
        charTex = &charIdleTex;
    }
    if (charTex->isNull()) {
        // Ultimate fallback - colored rectangle
        painter.fillRect(destRect, QColor(255, 0, 0));
        return;
    }

    painter.drawPixmap(destRect, *charTex);
}

void Game::renderItemInHand(QPainter& painter) {
    ItemType selectedItem = hotbar[selectedSlot];
    if (selectedItem == ITEM_NONE) return;

    // Calculate position next to player based on direction
    int screenX = (int)(playerX - cameraX);
    int screenY = (int)(playerY - cameraY);
    int offsetX = 0, offsetY = 0;

    // Position item based on facing direction - use CHARACTER_SIZE for proper positioning
    switch (playerDirection) {
    case DIR_UP:
        offsetX = CHARACTER_SIZE / 4;
        offsetY = -CHARACTER_SIZE / 3;  // Slightly above character
        break;
    case DIR_DOWN:
        offsetX = -CHARACTER_SIZE / 4;
        offsetY = CHARACTER_SIZE / 3;  // Slightly below character
        break;
    case DIR_LEFT:
        offsetX = -CHARACTER_SIZE / 3;  // Slightly to the left
        offsetY = CHARACTER_SIZE / 4;
        break;
    case DIR_RIGHT:
        offsetX = CHARACTER_SIZE / 3;  // Slightly to the right
        offsetY = CHARACTER_SIZE / 4;
        break;
    }

    // Item size should scale with character size
    int itemSize = CHARACTER_SIZE / 2;

    QRect destRect(
        screenX + CHARACTER_SIZE / 2 + offsetX - itemSize / 2,  // Center on character + offset
        screenY + CHARACTER_SIZE / 2 + offsetY - itemSize / 2,
        itemSize,
        itemSize
    );

    QPixmap* itemTex = nullptr;
    if (selectedItem == ITEM_HOE && !hoeTex.isNull()) {
        itemTex = &hoeTex;
    } else if (selectedItem == ITEM_SEED && !seedTex.isNull()) {
        itemTex = &seedTex;
    }

    if (itemTex) {
        painter.drawPixmap(destRect, *itemTex);
    }
}

QPixmap Game::loadTexture(const QString& path) {
    QPixmap pixmap(path);
    if (pixmap.isNull()) {
        std::cerr << "Unable to load image " << path.toStdString() << "!" << std::endl;
    }
    return pixmap;
}
