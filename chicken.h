#ifndef CHICKEN_H
#define CHICKEN_H

#include "ianimal.h"

class Chicken : public IAnimal
{
public:
    Chicken();
    ~Chicken();

    int getState() override;
    void setState(int newState) override;

    std::string getType() override;
    std::string getFoodType() override;

    bool getHarvestable() override;
    void setHarvestable() override;
};

#endif // CHICKEN_H
