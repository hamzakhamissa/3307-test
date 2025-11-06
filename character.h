#ifndef CHARACTER_H
#define CHARACTER_H
#include <vector>

// #include <QLabel>  // Qt not configured in CMakeLists.txt yet

#include "icrops.h"
#include "itool.h"

class Character
{
public:
    Character();
    ~Character();

    virtual int getState() = 0;
    virtual void setState() = 0;

    virtual int getX() = 0;
    virtual int getY() = 0;
    virtual void setX() = 0;
    virtual void setY() = 0;

    virtual int getSpeed() = 0;
    virtual void setSpeed() = 0;

    //Need a fix to inventory, currently cannot hold animals
    virtual std::vector<ICrops> getCropsInventory() = 0;
    virtual std::vector<ITool> getToolInventory() = 0;
    virtual void addCropsInventory(ICrops *crop) = 0;
    virtual void addToolInventory(ITool *tool) = 0;

    virtual void setMoney() = 0;
    virtual int getMoney() = 0;

    virtual void move() = 0;

    virtual bool isEquiped() = 0;

    virtual void interact() = 0;

    virtual void sleep() = 0;

private:
    int xPosition;
    int yPosition;
    int speed;
    int state;
    bool equiped;
    int money;
    std::vector<ICrops> cropsInventory;
    std::vector<ITool> toolInventory;
};

#endif // CHARACTER_H
