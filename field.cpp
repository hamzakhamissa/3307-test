#include "field.h"

Field::Field() {
    state = 0;  // 0 = grass (untilled), 1 = tilled, 2 = planted
    type = "Field";
    crops = nullptr;
    ownsCrop = false;
    watered = false;
}

Field::~Field() {
    // Clean up crop if we own it
    if (crops) {
        delete crops;
        crops = nullptr;
    }
}

int Field::getState() { return state; }

void Field::setState(int newState) {
    state = newState;
    // If state is reset to 0 (grass), remove crop if present
    if (state == 0 && crops) {
        delete crops;
        crops = nullptr;
        ownsCrop = false;
    }
}

std::string Field::getType() { return type; }
void Field::setType(std::string newType) { type = std::move(newType); }

void Field::setCrop(ICrops *crop) {
    // Clean up old crop if present
    if (crops && crops != crop) {
        delete crops;
    }
    crops = crop;
    updateOwnsCrop();
    if (crop) {
        state = 2; // Field is now planted
    }
}

ICrops *Field::getCrop() { return crops; }

bool Field::hasCrop() {
    updateOwnsCrop(); // Ensure flag is correct
    return ownsCrop;
}

bool Field::getWatered() { return watered; }

void Field::setWatered(bool newWatered) {
    watered = newWatered;
}

bool Field::canTill() const {
    return state == 0; // Can only till grass
}

bool Field::canPlant() const {
    return state == 1 && !ownsCrop; // Must be tilled and have no crop
}

bool Field::canWater() const {
    return state >= 1 && !watered; // Can water tilled or planted fields that aren't already watered
}

bool Field::canHarvest() const {
    return ownsCrop && crops && crops->getHarvestable();
}

void Field::updateOwnsCrop() {
    ownsCrop = (crops != nullptr);
}
