#include "inventory.h"

// InventorySlot constructors
InventorySlot::InventorySlot()
    : itemType(ItemTypeEnum::NONE), quantity(0)
{}

InventorySlot::InventorySlot(ItemTypeEnum type, int qty)
    : itemType(type), quantity(qty)
{}

// Inventory implementation
Inventory::Inventory()
{
    m_slots.resize(INVENTORY_SIZE);
    initializeDefault();
}

Inventory::~Inventory() = default;

void Inventory::initializeDefault()
{
    m_slots[0] = InventorySlot(ItemTypeEnum::HOE, 1);
    m_slots[1] = InventorySlot(ItemTypeEnum::WATERING_CAN, 1);

    for (int i = 2; i < INVENTORY_SIZE; ++i) {
        m_slots[i] = InventorySlot(ItemTypeEnum::NONE, 0);
    }

    m_itemCounts[ItemTypeEnum::HOE] = 1;
    m_itemCounts[ItemTypeEnum::WATERING_CAN] = 1;
}

bool Inventory::addItem(ItemTypeEnum itemType, int quantity)
{
    if (itemType == ItemTypeEnum::NONE || quantity <= 0) return false;

    int slotWithItem = findSlotWithItem(itemType);
    if (slotWithItem != -1) {
        m_slots[slotWithItem].quantity += quantity;
        m_itemCounts[itemType] += quantity;
        return true;
    }

    int emptySlot = findFirstEmptySlot();
    if (emptySlot == -1) return false;

    m_slots[emptySlot] = InventorySlot(itemType, quantity);
    m_itemCounts[itemType] = quantity;
    return true;
}

bool Inventory::removeItem(ItemTypeEnum itemType, int quantity)
{
    if (itemType == ItemTypeEnum::NONE || quantity <= 0) return false;

    int currentCount = getItemCount(itemType);
    if (currentCount < quantity) return false;

    int remaining = quantity;
    for (int i = 0; i < INVENTORY_SIZE && remaining > 0; ++i) {
        if (m_slots[i].itemType == itemType) {
            int toRemove = (remaining < m_slots[i].quantity) ? remaining : m_slots[i].quantity;
            m_slots[i].quantity -= toRemove;
            remaining -= toRemove;

            if (m_slots[i].quantity == 0) {
                m_slots[i] = InventorySlot(ItemTypeEnum::NONE, 0);
            }
        }
    }

    m_itemCounts[itemType] -= quantity;
    if (m_itemCounts[itemType] <= 0) {
        m_itemCounts.erase(itemType);
    }

    return true;
}

bool Inventory::removeItemFromSlot(int slot, int quantity)
{
    if (slot < 0 || slot >= INVENTORY_SIZE) return false;
    if (m_slots[slot].itemType == ItemTypeEnum::NONE) return false;
    if (m_slots[slot].quantity < quantity) return false;

    ItemTypeEnum itemType = m_slots[slot].itemType;
    m_slots[slot].quantity -= quantity;

    if (m_slots[slot].quantity == 0) {
        m_slots[slot] = InventorySlot(ItemTypeEnum::NONE, 0);
    }

    m_itemCounts[itemType] -= quantity;
    if (m_itemCounts[itemType] <= 0) {
        m_itemCounts.erase(itemType);
    }

    return true;
}

int Inventory::getItemCount(ItemTypeEnum itemType) const
{
    auto it = m_itemCounts.find(itemType);
    return (it != m_itemCounts.end()) ? it->second : 0;
}

int Inventory::getItemCountInSlot(int slot) const
{
    if (slot < 0 || slot >= INVENTORY_SIZE) return 0;
    return m_slots[slot].quantity;
}

ItemTypeEnum Inventory::getItemInSlot(int slot) const
{
    if (slot < 0 || slot >= INVENTORY_SIZE) return ItemTypeEnum::NONE;
    return m_slots[slot].itemType;
}

bool Inventory::isSlotEmpty(int slot) const
{
    if (slot < 0 || slot >= INVENTORY_SIZE) return true;
    return m_slots[slot].itemType == ItemTypeEnum::NONE;
}

bool Inventory::hasSpace() const
{
    return findFirstEmptySlot() != -1;
}

int Inventory::findFirstEmptySlot() const
{
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        if (m_slots[i].itemType == ItemTypeEnum::NONE) {
            return i;
        }
    }
    return -1;
}

int Inventory::findSlotWithItem(ItemTypeEnum itemType) const
{
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        if (m_slots[i].itemType == itemType) {
            return i;
        }
    }
    return -1;
}

const std::vector<InventorySlot>& Inventory::getSlots() const
{
    return m_slots;
}
