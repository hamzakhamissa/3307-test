#ifndef CHARACTER_H
#define CHARACTER_H

#include <map>
#include <Qt>        // Qt::Key
#include <QPainter>  // QPainter
#include <QPixmap>   // QPixmap
#pragma once

// Player facing direction
enum PlayerDirection { DIR_DOWN = 0, DIR_LEFT = 1, DIR_RIGHT = 2, DIR_UP = 3 };

// Inventory items (referenced by renderHeldItem)
enum ItemType { ITEM_NONE = 0, ITEM_HOE = 1, ITEM_SEED = 2, ITEM_WATERING_CAN = 3 };

class Character {
public:
    Character();

    // Update character movement/state from current keys and dt
    void updateFromKeys(const std::map<int, bool>& keys, float deltaTime);

    // Keep the character inside the map (in pixels)
    void clampToMap(int mapWidthTiles, int mapHeightTiles, int tileSize, int characterSize);

    // Compute the tile in front of the character
    void getFrontTileIndices(int tileSize, int characterSize, int& tileX, int& tileY) const;

    // Rendering (moved from Game)
    void renderCharacter(QPainter& painter,
                         const QPixmap& texDown,
                         const QPixmap& texLeft,
                         const QPixmap& texRight,
                         const QPixmap& texUp,
                         const QPixmap& texIdle,
                         float cameraX, float cameraY,
                         int characterSize) const;

    void renderHeldItem(QPainter& painter,
                        ItemType selectedItem,
                        const QPixmap& hoeTex,
                        const QPixmap& seedTex,
                        const QPixmap& wateringCanTex,
                        float cameraX, float cameraY,
                        int characterSize) const;

    // Accessors
    float x() const { return playerX; }
    float y() const { return playerY; }
    float speed() const { return playerSpeed; }
    bool  moving() const { return playerMoving; }
    PlayerDirection direction() const { return playerDirection; }

    // Mutators
    void setPosition(float x, float y) { playerX = x; playerY = y; }
    void setSpeed(float s) { playerSpeed = s; }

private:
    // Movement state
    float playerX;
    float playerY;
    float playerSpeed;
    PlayerDirection playerDirection;
    bool  playerMoving;
};

#endif // CHARACTER_H
