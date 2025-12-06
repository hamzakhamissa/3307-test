#ifndef ANIMALFACTORY_H
#define ANIMALFACTORY_H

#include "ianimal.h"
#include "itemtypeenum.h"
#include <string>

class AnimalFactory {
public:
    // Creates an animal by type name
    static IAnimal* createAnimal(const std::string& animalType);

    // Creates an animal from ItemTypeEnum
    static IAnimal* createAnimalFromItem(ItemTypeEnum itemType);

    // Check if a string represents a valid animal type
    static bool isValidAnimalType(const std::string& animalType);

    // Get the produce type for an animal
    static ItemTypeEnum getProduceType(const std::string& animalType);
};

#endif 
