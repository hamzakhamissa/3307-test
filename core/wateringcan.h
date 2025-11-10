#ifndef WATERINGCAN_H
#define WATERINGCAN_H

#include "itool.h"

class WateringCan : public ITool
{
public:
    WateringCan();
    ~WateringCan();

    std::string getType() override;
    int getState() override;
    void setState(int newState) override;
};

#endif // WATERINGCAN_H
