#include "iinteract.h"
#include "itiles.h"
#include "icrops.h"
#include "ianimal.h"
#include "player.h"
#include "itemtypeenum.h"
#include "field.h"
#include "cropfactory.h"
#include "animalfactory.h"
#include <iostream>

// ==================== TILLING ====================
ITiles* TillingInteraction::createReplacementTile(ITiles* oldTile) {
    if (!oldTile) return nullptr;

    if (oldTile->getType() == "Yard" && oldTile->getState() == 0) {
        Field* newField = new Field();
        newField->setState(1);
        return newField;
    }

    if (oldTile->getType() == "Field" && oldTile->getState() == 0) {
        oldTile->setState(1);
        return nullptr;
    }

    return nullptr;
}

bool TillingInteraction::doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) {
    if (!tile) return false;

    std::string tileType = tile->getType();
    if (tileType != "Field" && tileType != "Yard") return false;

    if (tile->getState() == 0) {
        tile->setState(1);
        return true;
    }

    return false;
}

bool TillingInteraction::checkState(ITiles* tile, ICrops* crop, IAnimal* animal) {
    if (!tile) return false;
    std::string tileType = tile->getType();
    return (tileType == "Field" || tileType == "Yard") && tile->getState() == 0;
}

bool TillingInteraction::isEquipped(Player* player) {
    if (!player) return false;
    return player->getSelectedItem() == ItemTypeEnum::HOE;
}

// ==================== PLANTING ====================
bool PlantingInteraction::doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) {
    if (!tile || !player) return false;
    if (tile->getType() != "Field" || tile->getState() != 1 || tile->hasCrop()) return false;

    ItemTypeEnum selectedItem = player->getSelectedItem();
    ICrops* newCrop = CropFactory::createCropFromSeed(selectedItem);

    if (!newCrop) return false;

    // Try to remove seed from inventory
    if (!player->removeItemFromInventory(selectedItem, 1)) {
        delete newCrop;
        return false;
    }

    tile->setCrop(newCrop);
    return true;
}

bool PlantingInteraction::checkState(ITiles* tile, ICrops* crop, IAnimal* animal) {
    if (!tile) return false;
    return tile->getType() == "Field" && tile->getState() == 1 && !tile->hasCrop();
}

bool PlantingInteraction::isEquipped(Player* player) {
    if (!player) return false;
    ItemTypeEnum item = player->getSelectedItem();
    return item == ItemTypeEnum::WHEAT_SEED ||
           item == ItemTypeEnum::TOMATO_SEED ||
           item == ItemTypeEnum::CORN_SEED;
}

// ==================== WATERING ====================
bool WateringInteraction::doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) {
    if (!tile || !player) return false;
    if (tile->getType() != "Field") return false;

    tile->setWatered(true);
    return true;
}

bool WateringInteraction::checkState(ITiles* tile, ICrops* crop, IAnimal* animal) {
    if (!tile) return false;
    return tile->getType() == "Field" && !tile->getWatered();
}

bool WateringInteraction::isEquipped(Player* player) {
    if (!player) return false;
    return player->getSelectedItem() == ItemTypeEnum::WATERING_CAN;
}

// ==================== HARVEST CROP ====================
bool HarvestCropInteraction::doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) {
    if (!tile || !crop || !player) return false;
    if (!crop->getHarvestable()) return false;

    // Determine what item to add based on crop type
    ItemTypeEnum harvestItem = ItemTypeEnum::NONE;
    if (crop->getType() == "Wheat") {
        harvestItem = ItemTypeEnum::WHEAT;
    } else if (crop->getType() == "Tomato") {
        harvestItem = ItemTypeEnum::TOMATO;
    } else if (crop->getType() == "Corn") {
        harvestItem = ItemTypeEnum::CORN;
    }

    if (harvestItem != ItemTypeEnum::NONE) {
        if (player->addItemToInventory(harvestItem, 1)) {
            tile->setCrop(nullptr);
            return true;
        }
    }
    return false;
}

bool HarvestCropInteraction::checkState(ITiles* tile, ICrops* crop, IAnimal* animal) {
    return crop && crop->getHarvestable();
}

bool HarvestCropInteraction::isEquipped(Player* player) {
    return true; // Can harvest with empty hands
}

// ==================== HARVEST ANIMAL ====================
bool HarvestAnimalInteraction::doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) {
    if (!animal || !player) return false;
    if (!animal->getHarvestable()) return false;

    // Determine what item to add based on animal type
    ItemTypeEnum harvestItem = AnimalFactory::getProduceType(animal->getType());

    if (harvestItem != ItemTypeEnum::NONE) {
        if (player->addItemToInventory(harvestItem, 1)) {
            animal->setState(0); // Reset to unfed state
            animal->setHarvestable(); // Update harvestable state
            return true;
        }
    }
    return false;
}

bool HarvestAnimalInteraction::checkState(ITiles* tile, ICrops* crop, IAnimal* animal) {
    return animal && animal->getHarvestable();
}

bool HarvestAnimalInteraction::isEquipped(Player* player) {
    return true; // Can harvest with empty hands
}

// ==================== FEED ANIMAL ====================
bool FeedAnimalInteraction::doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) {
    if (!animal || !player) return false;

    // Check if player has the correct food for this animal
    std::string foodType = animal->getFoodType();
    ItemTypeEnum requiredFood = ItemTypeEnum::NONE;

    if (foodType == "Wheat") {
        requiredFood = ItemTypeEnum::WHEAT;
    } else if (foodType == "Corn") {
        requiredFood = ItemTypeEnum::CORN;
    }

    // If player doesn't have the specific food, check for generic FEED
    if (requiredFood != ItemTypeEnum::NONE && player->getItemCount(requiredFood) > 0) {
        if (player->removeItemFromInventory(requiredFood, 1)) {
            animal->setState(1); // Fed state
            animal->setHarvestable(); // Make harvestable
            return true;
        }
    } else if (player->getItemCount(ItemTypeEnum::FEED) > 0) {
        if (player->removeItemFromInventory(ItemTypeEnum::FEED, 1)) {
            animal->setState(1); // Fed state
            animal->setHarvestable(); // Make harvestable
            return true;
        }
    }

    return false;
}

bool FeedAnimalInteraction::checkState(ITiles* tile, ICrops* crop, IAnimal* animal) {
    return animal != nullptr && animal->getState() == 0; // Can only feed unfed animals
}

bool FeedAnimalInteraction::isEquipped(Player* player) {
    if (!player) return false;
    return player->getItemCount(ItemTypeEnum::WHEAT) > 0 ||
           player->getItemCount(ItemTypeEnum::CORN) > 0 ||
           player->getItemCount(ItemTypeEnum::FEED) > 0;
}

// ==================== PLACE ANIMAL (NEW) ====================
bool PlaceAnimalInteraction::doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) {
    if (!tile || !player) return false;

    // Can only place animals on Yard tiles that don't have animals
    if (tile->getType() != "Yard" || tile->hasAnimal()) return false;

    ItemTypeEnum selectedItem = player->getSelectedItem();
    IAnimal* newAnimal = AnimalFactory::createAnimalFromItem(selectedItem);

    if (!newAnimal) return false;

    // Try to remove animal from inventory
    if (!player->removeItemFromInventory(selectedItem, 1)) {
        delete newAnimal;
        return false;
    }

    tile->setAnimal(newAnimal);
    std::cout << "Placed " << newAnimal->getType() << " on Yard tile" << std::endl;
    return true;
}

bool PlaceAnimalInteraction::checkState(ITiles* tile, ICrops* crop, IAnimal* animal) {
    if (!tile) return false;
    return tile->getType() == "Yard" && !tile->hasAnimal();
}

bool PlaceAnimalInteraction::isEquipped(Player* player) {
    if (!player) return false;
    ItemTypeEnum item = player->getSelectedItem();
    return item == ItemTypeEnum::COW || item == ItemTypeEnum::CHICKEN;
}
