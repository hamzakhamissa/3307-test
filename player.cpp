#include "player.h"
#include "inventory.h"
#include "character.h"
#include <map>

Player::Player()
    : money(500) // Starting money
    , state(0)
    , equipped(false)
    , selectedSlot(0)
{
    inventory = std::make_unique<Inventory>();
    character.setPosition(400.0f, 300.0f);
    character.setSpeed(100.0f);
}

Player::~Player() = default;

float Player::getX() const { return character.x(); }
float Player::getY() const { return character.y(); }
void Player::setX(float x) { character.setPosition(x, character.y()); }
void Player::setY(float y) { character.setPosition(character.x(), y); }
void Player::setPosition(float x, float y) { character.setPosition(x, y); }
float Player::getSpeed() const { return character.speed(); }
void Player::setSpeed(float speed) { character.setSpeed(speed); }

void Player::updateFromKeys(const std::map<int, bool>& keys, float deltaTime) {
    character.updateFromKeys(keys, deltaTime);
}

void Player::clampToMap(int mapWidthTiles, int mapHeightTiles, int tileSize, int characterSize) {
    character.clampToMap(mapWidthTiles, mapHeightTiles, tileSize, characterSize);
}

void Player::getFrontTileIndices(int tileSize, int characterSize, int& tileX, int& tileY) const {
    character.getFrontTileIndices(tileSize, characterSize, tileX, tileY);
}

PlayerDirection Player::direction() const { return character.direction(); }
bool Player::moving() const { return character.moving(); }

void Player::renderCharacter(QPainter& painter,
                             const QPixmap& texDown,
                             const QPixmap& texLeft,
                             const QPixmap& texRight,
                             const QPixmap& texUp,
                             const QPixmap& texIdle,
                             float cameraX, float cameraY,
                             int characterSize) const {
    character.renderCharacter(painter, texDown, texLeft, texRight, texUp, texIdle, cameraX, cameraY, characterSize);
}

void Player::renderHeldItem(QPainter& painter,
                            ItemType selectedItem,
                            const QPixmap& hoeTex,
                            const QPixmap& seedTex,
                            const QPixmap& wateringCanTex,
                            float cameraX, float cameraY,
                            int characterSize) const {
    character.renderHeldItem(painter, selectedItem, hoeTex, seedTex, wateringCanTex, cameraX, cameraY, characterSize);
}

bool Player::addItemToInventory(ItemTypeEnum itemType, int quantity) {
    return inventory->addItem(itemType, quantity);
}

bool Player::removeItemFromInventory(ItemTypeEnum itemType, int quantity) {
    return inventory->removeItem(itemType, quantity);
}

int Player::getItemCount(ItemTypeEnum itemType) const {
    return inventory->getItemCount(itemType);
}

ItemTypeEnum Player::getSelectedItem() const {
    if (selectedSlot < 0 || selectedSlot >= Inventory::INVENTORY_SIZE) {
        return ItemTypeEnum::NONE;
    }
    return inventory->getItemInSlot(selectedSlot);
}

void Player::setSelectedSlot(int slot) {
    if (slot >= 0 && slot < Inventory::INVENTORY_SIZE) {
        selectedSlot = slot;
    }
}

bool Player::spendMoney(int amount) {
    if (money >= amount) {
        money -= amount;
        return true;
    }
    return false;
}

void Player::interact() {
    // Interaction logic will be handled by interaction system
}

void Player::sleep() {
    // Sleep logic - advance day
    state = 0; // Reset state
}

ItemType Player::convertItemTypeEnumToItemType(ItemTypeEnum itemEnum) const {
    switch (itemEnum) {
        case ItemTypeEnum::HOE: return ITEM_HOE;
        case ItemTypeEnum::WATERING_CAN: return ITEM_WATERING_CAN;
        case ItemTypeEnum::WHEAT_SEED:
        case ItemTypeEnum::TOMATO_SEED:
        case ItemTypeEnum::CORN_SEED: return ITEM_SEED;
        default: return ITEM_NONE;
    }
}
