#include "character.h"

Character::Character()
    : playerX(400.0f)
    , playerY(300.0f)
    , playerSpeed(100.0f)
    , playerDirection(DIR_DOWN)
    , playerMoving(false)
{}

void Character::updateFromKeys(const std::map<int, bool>& keys, float deltaTime) {
    auto down = [&](int k){
        auto it = keys.find(k);
        return it != keys.end() && it->second;
    };

    float moveX = 0.0f, moveY = 0.0f;
    PlayerDirection newDir = playerDirection;

    // One-axis-only movement (vertical has priority over horizontal)
    if (down(Qt::Key_W) || down(Qt::Key_Up))           { moveY = -1.0f; newDir = DIR_UP; }
    else if (down(Qt::Key_S) || down(Qt::Key_Down))    { moveY =  1.0f; newDir = DIR_DOWN; }
    else if (down(Qt::Key_A) || down(Qt::Key_Left))    { moveX = -1.0f; newDir = DIR_LEFT; }
    else if (down(Qt::Key_D) || down(Qt::Key_Right))   { moveX =  1.0f; newDir = DIR_RIGHT; }

    playerMoving = (moveX != 0.0f || moveY != 0.0f);
    if (playerMoving) playerDirection = newDir;

    playerX += moveX * playerSpeed * deltaTime;
    playerY += moveY * playerSpeed * deltaTime;
}

void Character::clampToMap(int mapWidthTiles, int mapHeightTiles, int tileSize, int characterSize) {
    const float maxX = mapWidthTiles * tileSize - characterSize;
    const float maxY = mapHeightTiles * tileSize - characterSize;

    if (playerX < 0)       playerX = 0;
    if (playerY < 0)       playerY = 0;
    if (playerX > maxX)    playerX = maxX;
    if (playerY > maxY)    playerY = maxY;
}

void Character::getFrontTileIndices(int tileSize, int characterSize, int& tileX, int& tileY) const {
    const int centerTileX = static_cast<int>(playerX + characterSize / 2) / tileSize;
    const int centerTileY = static_cast<int>(playerY + characterSize / 2) / tileSize;

    switch (playerDirection) {
    case DIR_UP:    tileX = centerTileX;     tileY = centerTileY - 1; break;
    case DIR_DOWN:  tileX = centerTileX;     tileY = centerTileY + 1; break;
    case DIR_LEFT:  tileX = centerTileX - 1; tileY = centerTileY;     break;
    case DIR_RIGHT: tileX = centerTileX + 1; tileY = centerTileY;     break;
    }
}

void Character::renderCharacter(QPainter& painter,
                                const QPixmap& texDown,
                                const QPixmap& texLeft,
                                const QPixmap& texRight,
                                const QPixmap& texUp,
                                const QPixmap& texIdle,
                                float cameraX, float cameraY,
                                int characterSize) const
{
    const int screenX = static_cast<int>(playerX - cameraX);
    const int screenY = static_cast<int>(playerY - cameraY);
    QRect destRect(screenX, screenY, characterSize, characterSize);

    const QPixmap* charTex = &texIdle;
    if (playerMoving) {
        switch (playerDirection) {
        case DIR_DOWN:  charTex = &texDown;  break;
        case DIR_LEFT:  charTex = &texLeft;  break;
        case DIR_RIGHT: charTex = &texRight; break;
        case DIR_UP:    charTex = &texUp;    break;
        }
    }
    if (charTex && !charTex->isNull())
        painter.drawPixmap(destRect, *charTex);
    else
        painter.fillRect(destRect, QColor(255, 0, 0));
}

void Character::renderHeldItem(QPainter& painter,
                               ItemType selectedItem,
                               const QPixmap& hoeTex,
                               const QPixmap& seedTex,
                               const QPixmap& wateringCanTex,
                               float cameraX, float cameraY,
                               int characterSize) const
{
    if (selectedItem == ITEM_NONE) return;

    const int screenX = static_cast<int>(playerX - cameraX);
    const int screenY = static_cast<int>(playerY - cameraY);

    int offsetX = 0, offsetY = 0;
    switch (playerDirection) {
    case DIR_UP:    offsetX = characterSize / 4;  offsetY = -characterSize / 3; break;
    case DIR_DOWN:  offsetX = -characterSize / 4; offsetY =  characterSize / 3; break;
    case DIR_LEFT:  offsetX = -characterSize / 3; offsetY =  characterSize / 4; break;
    case DIR_RIGHT: offsetX =  characterSize / 3; offsetY =  characterSize / 4; break;
    }

    const int itemSize = characterSize / 2;
    QRect destRect(
        screenX + characterSize / 2 + offsetX - itemSize / 2,
        screenY + characterSize / 2 + offsetY - itemSize / 2,
        itemSize, itemSize
        );

    const QPixmap* itemTex = nullptr;
    if (selectedItem == ITEM_HOE && !hoeTex.isNull())                    itemTex = &hoeTex;
    else if (selectedItem == ITEM_SEED && !seedTex.isNull())            itemTex = &seedTex;
    else if (selectedItem == ITEM_WATERING_CAN && !wateringCanTex.isNull()) itemTex = &wateringCanTex;

    if (itemTex) painter.drawPixmap(destRect, *itemTex);
}
