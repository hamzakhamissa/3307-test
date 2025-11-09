#ifndef IANIMAL_H
#define IANIMAL_H

#include <string>

class IAnimal
{
public:
    IAnimal();
    virtual ~IAnimal();  // CRITICAL: Must be virtual for interface

    virtual int getState() = 0;
    virtual void setState(int newState) = 0;
    virtual std::string getType() = 0;
    virtual std::string getFoodType() = 0;
    virtual bool getHarvestable() = 0;
    virtual void setHarvestable() = 0;

protected:
    std::string type;
    int state;
    std::string foodType;
    bool harvestable;
};

#endif // IANIMAL_H
