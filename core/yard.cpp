#include "yard.h"

Yard::Yard() {
    state = 0;
    type = "Yard";
    crops = nullptr;
    ownsCrop = false;
    watered = false;
    animal = nullptr;
    ownsAnimal = false;
}

Yard::~Yard() {
    // Clean up animal if we own it
    if (animal && ownsAnimal) {
        delete animal;
        animal = nullptr;
    }
}

int Yard::getState() { return state; }
void Yard::setState(int newState) { state = newState; }

std::string Yard::getType() { return type; }
void Yard::setType(std::string newType) { type = std::move(newType); }

void Yard::setCrop(ICrops * /*crop*/) {
    crops = nullptr;
    ownsCrop = false;
}
ICrops *Yard::getCrop() { return nullptr; }
bool Yard::hasCrop() { return false; }

bool Yard::getWatered() { return watered; }
void Yard::setWatered(bool newWatered) { watered = newWatered; }

void Yard::setAnimal(IAnimal *newAnimal) {
    if (animal && animal != newAnimal && ownsAnimal) {
        delete animal;
    }
    animal = newAnimal;
    updateOwnsAnimal();
}

IAnimal *Yard::getAnimal() {
    return animal;
}

bool Yard::hasAnimal() {
    updateOwnsAnimal();
    return ownsAnimal;
}

void Yard::updateOwnsAnimal() {
    ownsAnimal = (animal != nullptr);
}
