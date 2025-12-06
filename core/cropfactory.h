#ifndef CROPFACTORY_H
#define CROPFACTORY_H

#include "icrops.h"
#include "itemtypeenum.h"
#include <string>

class CropFactory {
public:
    // Creates a crop by type name
    static ICrops* createCrop(const std::string& cropType);

    // Creates a crop from ItemTypeEnum
    static ICrops* createCropFromSeed(ItemTypeEnum seedType);

    // Check if a string represents a valid crop type
    static bool isValidCropType(const std::string& cropType);
};

#endif
