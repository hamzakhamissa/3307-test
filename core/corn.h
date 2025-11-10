#ifndef CORN_H
#define CORN_H

#include "icrops.h"

class Corn : public ICrops
{
public:
    Corn();
    ~Corn();

    int getState() override;
    void setState() override;

    std::string getType() override;

    int getGrowthSpeed() override;
    void setGrowthSpeed(int speed) override;

    void setHarvestable(bool ynHarvest) override;
    bool getHarvestable() override;
};

#endif // CORN_H
