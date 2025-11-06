#ifndef ICROPS_H
#define ICROPS_H
#include <string>

class ICrops
{
public:
    ICrops();
    ~ICrops();

    virtual int getState() = 0;
    virtual void setState() = 0;

    virtual std::string getType() = 0;

    virtual int getGrowthSpeed() = 0;
    virtual void setGrowthSpeed(int speed) = 0;

    virtual void setHarvestable (bool ynHarvest) = 0;
    virtual bool getHarvestable () = 0;

protected:
    std::string type;
    int state;
    int growthSpeed;
    bool harvestable;
};

#endif // ICROPS_H
