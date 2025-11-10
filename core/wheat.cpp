#include "wheat.h"

Wheat::Wheat()
{
    type = "Wheat";
    state = 0;
    growthSpeed = 1;
    harvestable = false;
}

/*
Setter for state, fully grown state set to 3, if below 3 it increments the state, if at 3 or more
the Wheat is harvestable
*/
void Wheat::setState()
{
    if (getState() < 3) {
        state += growthSpeed;
    }

    if (getState() >= 3) {
        setHarvestable(true);
    }
}

// Getter to return the type of crop, should always be Wheat
std::string Wheat::getType()
{
    return type;
}

// Gets the growth speed, this is how much the state is moved ahead by whenever the crop is watered
int Wheat::getGrowthSpeed()
{
    return growthSpeed;
}

// Sets the growth speed, this will only be used for balancing the game
void Wheat::setGrowthSpeed(int speed)
{
    growthSpeed = speed;
}

// Setter to set when a crop is ready to be harvested
void Wheat::setHarvestable(bool ynHarvest)
{
    harvestable = ynHarvest;
}

// Checks whether or not the crop is ready to be harvested
bool Wheat::getHarvestable()
{
    return harvestable;
}

// Getter for state
int Wheat::getState()
{
    return state;
}

// Destructor
Wheat::~Wheat() {}
