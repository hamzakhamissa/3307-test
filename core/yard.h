#ifndef YARD_H
#define YARD_H

#include "itiles.h"

// Non-plantable walking area
class Yard : public ITiles {
public:
    Yard();
    ~Yard();

    int getState() override;
    void setState(int newState) override;

    std::string getType() override;
    void setType(std::string newType) override;

    void setCrop(ICrops *crop) override; // no-op (not plantable)
    ICrops *getCrop() override;          // always nullptr
    bool hasCrop() override;             // always false

    bool getWatered() override;
    void setWatered(bool newWatered) override;
};

#endif // YARD_H
