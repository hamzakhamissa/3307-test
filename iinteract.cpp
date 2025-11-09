#include "iinteract.h"
#include "itiles.h"
#include "icrops.h"
#include "ianimal.h"
#include "player.h"
#include "itemtypeenum.h"
#include "field.h"
#include "wheat.h"
#include "tomato.h"
#include "corn.h"
#include "cow.h"
#include "chicken.h"
#include <iostream>

// In your iinteract.cpp implementation:
ITiles* TillingInteraction::createReplacementTile(ITiles* oldTile) {
    if (!oldTile) return nullptr;

    // If we're tilling a Yard, replace it with a Field
    if (oldTile->getType() == "Yard" && oldTile->getState() == 0) {
        Field* newField = new Field();
        newField->setState(1); // Set to tilled state
        return newField;
    }

    // If it's already a Field, just till it
    if (oldTile->getType() == "Field" && oldTile->getState() == 0) {
        oldTile->setState(1);
        return nullptr; // No replacement needed
    }

    return nullptr;
}

bool TillingInteraction::doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) {
    if (!tile) {
        std::cout << "TillingInteraction: tile is null!" << std::endl;
        return false;
    }

    std::string tileType = tile->getType();
    std::cout << "TillingInteraction: Tile type = " << tileType << std::endl;
    std::cout << "TillingInteraction: Tile state before = " << tile->getState() << std::endl;

    if (tileType != "Field" && tileType != "Yard") {
        std::cout << "TillingInteraction: Wrong tile type!" << std::endl;
        return false;
    }

    if (tile->getState() == 0) {
        std::cout << "TillingInteraction: Setting state to 1..." << std::endl;
        tile->setState(1); // Till the ground
        std::cout << "TillingInteraction: Tile state after = " << tile->getState() << std::endl;
        return true;
    }

    std::cout << "TillingInteraction: State is not 0!" << std::endl;
    return false;
}


bool TillingInteraction::checkState(ITiles* tile, ICrops* crop, IAnimal* animal) {
    if (!tile) return false;
    
    // Allow tilling on both Field and Yard tiles when they're in grass state (state 0)
    std::string tileType = tile->getType();
    return (tileType == "Field" || tileType == "Yard") && tile->getState() == 0;
}

bool TillingInteraction::isEquipped(Player* player) {
    if (!player) return false;
    return player->getSelectedItem() == ItemTypeEnum::HOE;
}

// Planting Interaction
bool PlantingInteraction::doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) {
    if (!tile || !player) return false;
    if (tile->getType() != "Field" || tile->getState() != 1 || tile->hasCrop()) return false;

    ItemTypeEnum selectedItem = player->getSelectedItem();
    ICrops* newCrop = nullptr;

    // Create crop based on seed type
    if (selectedItem == ItemTypeEnum::WHEAT_SEED) {
        newCrop = new Wheat();
        if (!player->removeItemFromInventory(ItemTypeEnum::WHEAT_SEED, 1)) {
            delete newCrop;
            return false;
        }
    } else if (selectedItem == ItemTypeEnum::TOMATO_SEED) {
        newCrop = new Tomato();
        if (!player->removeItemFromInventory(ItemTypeEnum::TOMATO_SEED, 1)) {
            delete newCrop;
            return false;
        }
    } else if (selectedItem == ItemTypeEnum::CORN_SEED) {
        newCrop = new Corn();
        if (!player->removeItemFromInventory(ItemTypeEnum::CORN_SEED, 1)) {
            delete newCrop;
            return false;
        }
    } else {
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

// Watering Interaction
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

// Harvest Crop Interaction
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
            // CRITICAL FIX: Let tile handle crop deletion
            // tile->setCrop(nullptr) will delete the crop internally
            tile->setCrop(nullptr);
            // DO NOT delete crop here - tile owns it and will clean it up!
            return true;
        }
    }
    return false;
}

bool HarvestCropInteraction::checkState(ITiles* tile, ICrops* crop, IAnimal* animal) {
    return crop && crop->getHarvestable();
}

bool HarvestCropInteraction::isEquipped(Player* player) {
    // Can harvest with empty hands or any tool
    return true;
}

// Harvest Animal Interaction
bool HarvestAnimalInteraction::doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) {
    if (!animal || !player) return false;
    if (!animal->getHarvestable()) return false;

    // Determine what item to add based on animal type
    ItemTypeEnum harvestItem = ItemTypeEnum::NONE;
    if (animal->getType() == "Cow") {
        harvestItem = ItemTypeEnum::MILK;
    } else if (animal->getType() == "Chicken") {
        harvestItem = ItemTypeEnum::EGG;
    }

    if (harvestItem != ItemTypeEnum::NONE) {
        if (player->addItemToInventory(harvestItem, 1)) {
            animal->setHarvestable(); // Reset harvestable state
            return true;
        }
    }
    return false;
}

bool HarvestAnimalInteraction::checkState(ITiles* tile, ICrops* crop, IAnimal* animal) {
    return animal && animal->getHarvestable();
}

bool HarvestAnimalInteraction::isEquipped(Player* player) {
    // Can harvest with empty hands
    return true;
}

// Feed Animal Interaction
bool FeedAnimalInteraction::doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) {
    if (!animal || !player) return false;

    // Check if player has feed
    if (player->getItemCount(ItemTypeEnum::FEED) > 0) {
        // Check if animal likes this food type
        // For now, all animals accept feed
        if (player->removeItemFromInventory(ItemTypeEnum::FEED, 1)) {
            animal->setState(1); // Fed state
            animal->setHarvestable(); // Make harvestable
            return true;
        }
    }
    return false;
}

bool FeedAnimalInteraction::checkState(ITiles* tile, ICrops* crop, IAnimal* animal) {
    return animal != nullptr;
}

bool FeedAnimalInteraction::isEquipped(Player* player) {
    if (!player) return false;
    return player->getItemCount(ItemTypeEnum::FEED) > 0;
}

