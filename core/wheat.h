#ifndef WHEAT_H
#define WHEAT_H

#include "icrops.h"

class Wheat : public ICrops
{
public:
    Wheat();
    ~Wheat();

    int getState() override;
    void setState() override;

    std::string getType() override;

    int getGrowthSpeed() override;
    void setGrowthSpeed(int speed) override;

    void setHarvestable(bool ynHarvest) override;
    bool getHarvestable() override;
};

#endif // WHEAT_h
