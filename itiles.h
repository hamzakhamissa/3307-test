#ifndef ITILES_H
#define ITILES_H

#include <string>
#include "icrops.h"

// Minimal tile interface shared by Yard (non-plantable) and Field (plantable)
class ITiles {
public:
    ITiles();
    virtual ~ITiles(); // FIXED: Added 'virtual' keyword - CRITICAL for polymorphic deletion

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
    std::string type{};
    int state{0};
    bool ownsCrop{false};
    bool watered{false};
    ICrops *crops{nullptr};
};

#endif // ITILES_H
