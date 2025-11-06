#include "field.h"

// Constructor for field object, sets appropriate attributes, uses setter injection
Field::Field()
{
    state = 0;
    type = "Field";
    crops = nullptr;
    ownsCrop = false;
    watered = false;
}

// Gets the state of the itile object
int Field::getState()
{
    return state;
}

// Sets new state for the itile object
void Field::setState(int newstate)
{
    state = newstate;
}

// Gets the type of the itile object, should always be field
std::string Field::getType()
{
    return type;
}

// Sets type of itile object, shouldn't have to be used
void Field::setType(std::string newType)
{
    type = newType;
}

// Gets the current crop that the itile object holds
ICrops *Field::getCrop()
{
    return crops;
}

// Sets the crop that the itile object will hold
void Field::setCrop(ICrops *crop)
{
    crops = crop;
    ownsCrop = (crop != nullptr);
}

// Checks if the itile object has a crop
bool Field::hasCrop()
{
    return ownsCrop;
}

// Checks if the itile object was watered
bool Field::getWatered()
{
    return watered;
}

// Sets the itile object to be watered when the watering can is used on it, or to false when the day ends
void Field::setWatered(bool newWatered)
{
    watered = newWatered;
}
