#include "animalfactory.h"
#include "cow.h"
#include "chicken.h"
#include <iostream>

IAnimal* AnimalFactory::createAnimal(const std::string& animalType) {
    if (animalType == "Cow") {
        return new Cow();
    } else if (animalType == "Chicken") {
        return new Chicken();
    }

    std::cerr << "AnimalFactory: Unknown animal type: " << animalType << std::endl;
    return nullptr;
}

IAnimal* AnimalFactory::createAnimalFromItem(ItemTypeEnum itemType) {
    switch (itemType) {
    case ItemTypeEnum::COW:
        return new Cow();
    case ItemTypeEnum::CHICKEN:
        return new Chicken();
    default:
        std::cerr << "AnimalFactory: Invalid item type: " << static_cast<int>(itemType) << std::endl;
        return nullptr;
    }
}

bool AnimalFactory::isValidAnimalType(const std::string& animalType) {
    return animalType == "Cow" || animalType == "Chicken";
}

ItemTypeEnum AnimalFactory::getProduceType(const std::string& animalType) {
    if (animalType == "Cow") {
        return ItemTypeEnum::MILK;
    } else if (animalType == "Chicken") {
        return ItemTypeEnum::EGG;
    }
    return ItemTypeEnum::NONE;
}
