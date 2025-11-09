#ifndef FIELD_H
#define FIELD_H

#include "itiles.h"

// Plantable tile - State: 0=grass, 1=tilled, 2=planted
class Field : public ITiles {
public:
    Field();
    ~Field();

    int getState() override;
    void setState(int newState) override;

    std::string getType() override;
    void setType(std::string newType) override;

    void setCrop(ICrops *crop) override; // updates ownsCrop
    ICrops *getCrop() override;
    bool hasCrop() override;

    bool getWatered() override;
    void setWatered(bool newWatered) override;

    // Additional helper methods for stronger Field class
    bool canTill() const;      // Can this field be tilled?
    bool canPlant() const;      // Can a crop be planted here?
    bool canWater() const;      // Can this field be watered?
    bool canHarvest() const;    // Is there a harvestable crop?

private:
    void updateOwnsCrop();      // Internal helper to sync ownsCrop flag
};

#endif // FIELD_H
