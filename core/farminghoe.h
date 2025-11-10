#ifndef FARMINGHOE_H
#define FARMINGHOE_H

#include "itool.h"

class FarmingHoe : public ITool
{
public:
    FarmingHoe();
    ~FarmingHoe();

    std::string getType() override;
    int getState() override;
    void setState(int newState) override;
};

#endif // FARMINGHOE_H
