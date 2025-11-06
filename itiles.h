#ifndef ITILES_H
#define ITILES_H
#include <string>

#include "icrops.h"

class ITiles
{
public:
    ITiles();
    ~ITiles();

    virtual int getState() = 0;
    virtual void setState(int newstate) = 0;

    virtual std::string getType() = 0;
    virtual void setType(std::string newtype) = 0;

    virtual void setCrop(ICrops *crop) = 0;
    virtual ICrops *getCrop() = 0;
    virtual bool hasCrop() = 0;

    virtual bool getWatered() = 0;
    virtual void setWatered(bool newWatered) = 0;

protected:
    std::string type;
    int state;
    bool ownsCrop;
    bool watered;
    ICrops *crops;
};

#endif // ITILES_H
