#ifndef MARKET_H
#define MARKET_H

#include <map>
#include <vector>
#include <string>
#include "itemtypeenum.h"

// Forward declaration
class Player;

struct MarketItem {
    ItemTypeEnum itemType;
    int buyPrice;
    int sellPrice;
    std::string name;

    MarketItem(ItemTypeEnum type, int buy, int sell, const std::string& n)
        : itemType(type), buyPrice(buy), sellPrice(sell), name(n) {}
};

class Market {
public:
    Market();
    ~Market();

    // Buy an item
    bool buy(Player* player, ItemTypeEnum itemType, int quantity = 1);
    
    // Sell an item
    bool sell(Player* player, ItemTypeEnum itemType, int quantity = 1);
    
    // Check if item can be bought/sold
    bool canBuy(ItemTypeEnum itemType) const;
    bool canSell(ItemTypeEnum itemType) const;
    
    // Get prices
    int getBuyPrice(ItemTypeEnum itemType) const;
    int getSellPrice(ItemTypeEnum itemType) const;
    
    // Get all available items for buying
    std::vector<MarketItem> getBuyableItems() const;
    
    // Get all sellable items from player inventory
    std::vector<MarketItem> getSellableItems(Player* player) const;
    
    // Check type (for UI display)
    int checkType(ItemTypeEnum itemType) const; // Returns 0=seed, 1=crop, 2=tool, 3=animal, 4=product

private:
    std::map<ItemTypeEnum, MarketItem> items;
    void initializeItems();
};

#endif // MARKET_H

