#include "corn.h"
#include <string>

// Constructor for the Corn class, sets attributes, takes no parameters
Corn::Corn()
{
    type = "Corn";
    state = 0;
    growthSpeed = 1;
    harvestable = false;
}

// FIXED: Added missing destructor implementation
Corn::~Corn()
{
    // Nothing special needed
}

// Getter for state
int Corn::getState()
{
    return state;
}
void Corn::setState()
{
    if (getState() < 4) {
        state += growthSpeed;
    }
    if (getState() >= 4) {
        setHarvestable(true);
    }
}

// Getter to return the type of crop, should always be Corn
std::string Corn::getType()
{
    return type;
}

// Gets the growth speed, this is how much the state is moved ahead by whenever the crop is watered
int Corn::getGrowthSpeed()
{
    return growthSpeed;
}

// Sets the growth speed, this will only be used for balancing the game
void Corn::setGrowthSpeed(int speed)
{
    growthSpeed = speed;
}

// Setter to set when a crop is ready to be harvested
void Corn::setHarvestable(bool ynHarvest)
{
    harvestable = ynHarvest;
}

// Checks whether or not the crop is ready to be harvested
bool Corn::getHarvestable()
{
    return harvestable;
}
