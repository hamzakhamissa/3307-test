#include "yard.h"

Yard::Yard() {
    state = 0;
    type = "Yard";
    crops = nullptr;
    ownsCrop = false;
    watered = false;
}

int Yard::getState() { return state; }
void Yard::setState(int newState) { state = newState; }

std::string Yard::getType() { return type; }
void Yard::setType(std::string newType) { type = std::move(newType); }

// Not plantable: ignore crop assignment and always report no crop
void Yard::setCrop(ICrops * /*crop*/) {
    crops = nullptr;
    ownsCrop = false;
}
ICrops *Yard::getCrop() { return nullptr; }
bool Yard::hasCrop() { return false; }

bool Yard::getWatered() { return watered; }
void Yard::setWatered(bool newWatered) { watered = newWatered; }

Yard::~Yard() {}
