#ifndef ITILES_H
#define ITILES_H

#include <string>
#include "icrops.h"
#include "ianimal.h"

class ITiles {
public:
    ITiles();
    virtual ~ITiles();

    virtual int getState() = 0;
    virtual void setState(int newstate) = 0;
    virtual std::string getType() = 0;
    virtual void setType(std::string newtype) = 0;

    // Crop methods
    virtual void setCrop(ICrops *crop) = 0;
    virtual ICrops *getCrop() = 0;
    virtual bool hasCrop() = 0;
    virtual bool getWatered() = 0;
    virtual void setWatered(bool newWatered) = 0;

    // Animal methods
    virtual void setAnimal(IAnimal *animal) = 0;
    virtual IAnimal *getAnimal() = 0;
    virtual bool hasAnimal() = 0;

protected:
    std::string type{};
    int state{0};
    bool ownsCrop{false};
    bool watered{false};
    ICrops *crops{nullptr};
    IAnimal *animal{nullptr};
    bool ownsAnimal{false};
};

#endif 