#ifndef TOMATO_h
#define TOMATO_h

#include "icrops.h"

class Tomato : public ICrops
{
public:
    Tomato();
    ~Tomato();

    int getState() override;
    void setState() override;

    std::string getType() override;

    int getGrowthSpeed() override;
    void setGrowthSpeed(int speed) override;

    void setHarvestable(bool ynHarvest) override;
    bool getHarvestable() override;
};

#endif // TOMATO_h
