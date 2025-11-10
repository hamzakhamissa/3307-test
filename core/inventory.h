#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <map>
#include "itemtypeenum.h"

// Inventory slot structure
struct InventorySlot {
    ItemTypeEnum itemType;
    int quantity;

    InventorySlot();
    InventorySlot(ItemTypeEnum type, int qty);
};

class Inventory {
public:
    static const int INVENTORY_SIZE = 9;

    Inventory();
    ~Inventory();

    bool addItem(ItemTypeEnum itemType, int quantity = 1);
    bool removeItem(ItemTypeEnum itemType, int quantity = 1);
    bool removeItemFromSlot(int slot, int quantity = 1);

    int getItemCount(ItemTypeEnum itemType) const;
    int getItemCountInSlot(int slot) const;
    ItemTypeEnum getItemInSlot(int slot) const;

    bool isSlotEmpty(int slot) const;
    bool hasSpace() const;
    int findFirstEmptySlot() const;
    int findSlotWithItem(ItemTypeEnum itemType) const;

    void initializeDefault();

    const std::vector<InventorySlot>& getSlots() const;

private:
    std::vector<InventorySlot> m_slots;
    std::map<ItemTypeEnum, int> m_itemCounts;
};

#endif // INVENTORY_H
