#ifndef YARD_H
#define YARD_H

#include "itiles.h"

class Yard : public ITiles {
public:
    Yard();
    ~Yard();

    int getState() override;
    void setState(int newState) override;

    std::string getType() override;
    void setType(std::string newType) override;

    void setCrop(ICrops *crop) override;
    ICrops *getCrop() override;
    bool hasCrop() override;

    bool getWatered() override;
    void setWatered(bool newWatered) override;

    void setAnimal(IAnimal *animal) override;
    IAnimal *getAnimal() override;
    bool hasAnimal() override;

private:
    void updateOwnsAnimal();
};

#endif
