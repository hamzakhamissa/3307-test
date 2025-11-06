#include "cow.h"

// Constructor for the cow class sets type to cow and food to wheat
Cow::Cow()
{
    type = "Cow";
    foodType = "Wheat";
    state = 0;
    harvestable = false;
}

// Returns the current state of the cow
int Cow::getState()
{
    return state;
}

// Sets the state of the cow
void Cow::setState(int newState)
{
    state = newState;
}

// Gets the type of the animal. Should always be Cow
std::string Cow::getType()
{
    return type;
}

// Gets the food type of the animal
std::string Cow::getFoodType()
{
    return foodType;
}

// Checks whether or not the animal is harvestable
bool Cow::getHarvestable()
{
    return harvestable;
}

// Checks to see if the conditions are met for the cow to be harvestable (state == 1 (cow has been given food))
void Cow::setHarvestable()
{
    if (state == 1) {
        harvestable = true;
    } else {
        harvestable = false;
    }
}

// Destructor
Cow::~Cow() {}
