#include "yard.h"
/* Constructor for the water class, sets appropriate attributes, acts as an obsatcle,

all attributes should stay the same.
*/
Yard::Yard()
{
    state = 0;
    type = "Yard";
    crops = nullptr;
    ownsCrop = false;
    watered = false;
}

// Gets the state of the itile object
int Yard::getState()
{
    return state;
}

// Sets new state for the itile object
void Yard::setState(int newstate)
{
    state = newstate;
}

// Gets the type of the itile object, should always be yard
std::string Yard::getType()
{
    return type;
}

// Sets type of itile object, shouldn't have to be used
void Yard::setType(std::string newType)
{
    type = newType;
}

// Gets the current crop that the itile object holds
ICrops *Yard::getCrop()
{
    return crops;
}

// Sets the crop that the itile object will hold
void Yard::setCrop(ICrops *crop)
{
    crops = crop;
}

// Checks if the itile object has a crop
bool Yard::hasCrop()
{
    return ownsCrop;
}

// Checks if the itile object was watered
bool Yard::getWatered()
{
    return watered;
}

// Sets the itile object to be watered when the watering can is used on it, or to false when the day ends
void Yard::setWatered(bool newWatered)
{
    watered = newWatered;
}
