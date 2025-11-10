#ifndef COW_H
#define COW_H

#include "ianimal.h"

class Cow : public IAnimal
{
public:
    Cow();
    ~Cow();

    int getState() override;
    void setState(int newState) override;

    std::string getType() override;
    std::string getFoodType() override;

    bool getHarvestable() override;
    void setHarvestable() override;
};

#endif // COW_H
