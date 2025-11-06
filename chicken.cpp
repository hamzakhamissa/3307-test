#include "chicken.h"

// Constructor for the chicken class, sets appropriate attributes
Chicken::Chicken()
{
    type = "Chicken";
    foodType = "Corn";
    state = 0;
    harvestable = false;
}

// Returns the current state of the chicken
int Chicken::getState()
{
    return state;
}

// Sets the state of the cow
void Chicken::setState(int newState)
{
    state = newState;
}

// Gets the type of the animal. Should always be Cow
std::string Chicken::getType()
{
    return type;
}

// Gets the food type of the animal
std::string Chicken::getFoodType()
{
    return foodType;
}

// Checks whether or not the animal is harvestable
bool Chicken::getHarvestable()
{
    return harvestable;
}

// Checks to see if the conditions are met for the cow to be harvestable (state == 1 (chicken has been given food))
void Chicken::setHarvestable()
{
    if (state == 1) {
        harvestable = true;
    } else {
        harvestable = false;
    }
}

// Destructor
Chicken::~Chicken() {}
