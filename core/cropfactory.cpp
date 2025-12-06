#include "cropfactory.h"
#include "wheat.h"
#include "tomato.h"
#include "corn.h"
#include <iostream>

ICrops* CropFactory::createCrop(const std::string& cropType) {
    if (cropType == "Wheat") {
        return new Wheat();
    } else if (cropType == "Tomato") {
        return new Tomato();
    } else if (cropType == "Corn") {
        return new Corn();
    }

    std::cerr << "CropFactory: Unknown crop type: " << cropType << std::endl;
    return nullptr;
}

ICrops* CropFactory::createCropFromSeed(ItemTypeEnum seedType) {
    switch (seedType) {
    case ItemTypeEnum::WHEAT_SEED:
        return new Wheat();
    case ItemTypeEnum::TOMATO_SEED:
        return new Tomato();
    case ItemTypeEnum::CORN_SEED:
        return new Corn();
    default:
        std::cerr << "CropFactory: Invalid seed type: " << static_cast<int>(seedType) << std::endl;
        return nullptr;
    }
}

bool CropFactory::isValidCropType(const std::string& cropType) {
    return cropType == "Wheat" || cropType == "Tomato" || cropType == "Corn";
}
