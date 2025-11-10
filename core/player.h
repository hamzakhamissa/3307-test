#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <memory>
#include <map>
#include <QPainter>
#include <QPixmap>
#include "character.h"
#include "itemtypeenum.h"

// Forward declarations
class Inventory;
class Market;

class Player {
public:
    Player();
    ~Player();

    // Position and movement (delegates to Character)
    float getX() const;
    float getY() const;
    void setX(float x);
    void setY(float y);
    void setPosition(float x, float y);
    float getSpeed() const;
    void setSpeed(float speed);
    void updateFromKeys(const std::map<int, bool>& keys, float deltaTime);
    void clampToMap(int mapWidthTiles, int mapHeightTiles, int tileSize, int characterSize);
    void getFrontTileIndices(int tileSize, int characterSize, int& tileX, int& tileY) const;
    PlayerDirection direction() const;
    bool moving() const;

    // Rendering (delegates to Character)
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

    // Inventory
    Inventory* getInventory() { return inventory.get(); }
    bool addItemToInventory(ItemTypeEnum itemType, int quantity = 1);
    bool removeItemFromInventory(ItemTypeEnum itemType, int quantity = 1);
    int getItemCount(ItemTypeEnum itemType) const;
    ItemTypeEnum getSelectedItem() const;
    void setSelectedSlot(int slot);
    int getSelectedSlot() const { return selectedSlot; }

    // Money
    int getMoney() const { return money; }
    void setMoney(int amount) { money = amount; }
    void addMoney(int amount) { money += amount; }
    bool spendMoney(int amount);

    // State
    int getState() const { return state; }
    void setState(int newState) { state = newState; }
    bool isEquipped() const { return equipped; }
    void setEquipped(bool eq) { equipped = eq; }

    // Actions
    void interact();
    void sleep();

    // Helper
    ItemType convertItemTypeEnumToItemType(ItemTypeEnum itemEnum) const;

private:
    Character character;
    std::unique_ptr<Inventory> inventory;
    int money;
    int state;
    bool equipped;
    int selectedSlot; // 0-8 for inventory slots
};

#endif // PLAYER_H
