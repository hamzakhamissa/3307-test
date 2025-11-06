#ifndef WATER_H
#define WATER_H

#include "itiles.h"

class Water : ITiles
{
public:
    Water();
    ~Water();

    int getState() override;
    void setState(int newState) override;

    std::string getType() override;
    void setType(std::string newType) override;

    void setCrop(ICrops *crop) override;
    ICrops *getCrop() override;
    bool hasCrop() override;

    bool getWatered() override;
    void setWatered(bool newWatered) override;
};

#endif // WATER_H
