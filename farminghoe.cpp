#include "farminghoe.h"

// Constructor of the farming hoe class, sets appropriate attributes
FarmingHoe::FarmingHoe()
{
    type = "Hoe";
    state = 1;
}

// Gets the type of the object, should always be Hoe
std::string FarmingHoe::getType()
{
    return type;
}

// Gets the state of the object
int FarmingHoe::getState()
{
    return state;
}

// Sets a new state for the object
void FarmingHoe::setState(int newState)
{
    state = newState;
}

// Destructor
FarmingHoe::~FarmingHoe() {}
