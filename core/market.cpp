#include "market.h"
#include "player.h"
#include "inventory.h"  // ADDED: Need this to use Inventory methods
#include <algorithm>

Market::Market() {
    initializeItems();
}

Market::~Market() = default;

void Market::initializeItems() {
    // Use insert() instead of operator[] to avoid default construction
    // Seeds (buyable)
    items.insert({ItemTypeEnum::WHEAT_SEED, MarketItem(ItemTypeEnum::WHEAT_SEED, 10, 0, "Wheat Seed")});
    items.insert({ItemTypeEnum::TOMATO_SEED, MarketItem(ItemTypeEnum::TOMATO_SEED, 15, 0, "Tomato Seed")});
    items.insert({ItemTypeEnum::CORN_SEED, MarketItem(ItemTypeEnum::CORN_SEED, 12, 0, "Corn Seed")});

    // Animals (buyable)
    items.insert({ItemTypeEnum::COW, MarketItem(ItemTypeEnum::COW, 100, 0, "Cow")});
    items.insert({ItemTypeEnum::CHICKEN, MarketItem(ItemTypeEnum::CHICKEN, 50, 0, "Chicken")});

    // Crops (sellable)
    items.insert({ItemTypeEnum::WHEAT, MarketItem(ItemTypeEnum::WHEAT, 0, 20, "Wheat")});
    items.insert({ItemTypeEnum::TOMATO, MarketItem(ItemTypeEnum::TOMATO, 0, 30, "Tomato")});
    items.insert({ItemTypeEnum::CORN, MarketItem(ItemTypeEnum::CORN, 0, 25, "Corn")});

    // Animal products (sellable)
    items.insert({ItemTypeEnum::MILK, MarketItem(ItemTypeEnum::MILK, 0, 40, "Milk")});
    items.insert({ItemTypeEnum::EGG, MarketItem(ItemTypeEnum::EGG, 0, 35, "Egg")});

    // Note: Tools (HOE, WATERING_CAN) are NOT in the shop - they come on spawn
}

bool Market::buy(Player* player, ItemTypeEnum itemType, int quantity) {
    if (!player || quantity <= 0) return false;

    auto it = items.find(itemType);
    if (it == items.end() || it->second.buyPrice == 0) return false;

    int totalCost = it->second.buyPrice * quantity;
    if (!player->spendMoney(totalCost)) return false;

    // Check if inventory has space
    if (!player->getInventory()->hasSpace() && player->getItemCount(itemType) == 0) {
        // Refund money
        player->addMoney(totalCost);
        return false;
    }

    // Add items to inventory
    for (int i = 0; i < quantity; ++i) {
        if (!player->addItemToInventory(itemType, 1)) {
            // Refund remaining money
            player->addMoney(it->second.buyPrice * (quantity - i));
            return false;
        }
    }

    return true;
}

bool Market::sell(Player* player, ItemTypeEnum itemType, int quantity) {
    if (!player || quantity <= 0) return false;

    auto it = items.find(itemType);
    if (it == items.end() || it->second.sellPrice == 0) return false;

    // Check if player has enough items
    if (player->getItemCount(itemType) < quantity) return false;

    // Remove items from inventory
    if (!player->removeItemFromInventory(itemType, quantity)) return false;

    // Give money
    int totalEarnings = it->second.sellPrice * quantity;
    player->addMoney(totalEarnings);

    return true;
}

bool Market::canBuy(ItemTypeEnum itemType) const {
    auto it = items.find(itemType);
    return it != items.end() && it->second.buyPrice > 0;
}

bool Market::canSell(ItemTypeEnum itemType) const {
    auto it = items.find(itemType);
    return it != items.end() && it->second.sellPrice > 0;
}

int Market::getBuyPrice(ItemTypeEnum itemType) const {
    auto it = items.find(itemType);
    return (it != items.end()) ? it->second.buyPrice : 0;
}

int Market::getSellPrice(ItemTypeEnum itemType) const {
    auto it = items.find(itemType);
    return (it != items.end()) ? it->second.sellPrice : 0;
}

std::vector<MarketItem> Market::getBuyableItems() const {
    std::vector<MarketItem> buyable;
    for (const auto& pair : items) {
        // Exclude tools (HOE, WATERING_CAN) - they come on spawn
        if (pair.second.buyPrice > 0 && 
            pair.first != ItemTypeEnum::HOE && 
            pair.first != ItemTypeEnum::WATERING_CAN) {
            buyable.push_back(pair.second);
        }
    }
    return buyable;
}

std::vector<MarketItem> Market::getSellableItems(Player* player) const {
    std::vector<MarketItem> sellable;
    if (!player) return sellable;

    for (const auto& pair : items) {
        if (pair.second.sellPrice > 0 && player->getItemCount(pair.first) > 0) {
            sellable.push_back(pair.second);
        }
    }
    return sellable;
}

int Market::checkType(ItemTypeEnum itemType) const {
    // 0 = seed, 1 = crop, 2 = tool, 3 = animal, 4 = product
    if (itemType == ItemTypeEnum::WHEAT_SEED || itemType == ItemTypeEnum::TOMATO_SEED ||
        itemType == ItemTypeEnum::CORN_SEED) {
        return 0; // Seed
    } else if (itemType == ItemTypeEnum::WHEAT || itemType == ItemTypeEnum::TOMATO ||
               itemType == ItemTypeEnum::CORN) {
        return 1; // Crop
    } else if (itemType == ItemTypeEnum::HOE || itemType == ItemTypeEnum::WATERING_CAN) {
        return 2; // Tool
    } else if (itemType == ItemTypeEnum::MILK || itemType == ItemTypeEnum::EGG) {
        return 4; // Product
    }
    return -1; // Unknown
}
