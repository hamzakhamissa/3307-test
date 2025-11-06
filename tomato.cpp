#include "tomato.h"

Tomato::Tomato() {
    type = "Tomato";
    state = 0;
    growthSpeed = 2;
    harvestable = false;
}

/*
Setter for state, fully grown state set to 4, if below 4 it increments the state, if at 4 or more
the tomato is harvestable
*/
void Tomato::setState() {
    if (getState() < 4){
        state += growthSpeed;
    }

    if (getState() >= 4){
        setHarvestable(true);
    }
}

// Getter to return the type of crop, should always be Tomato
std::string Tomato::getType() {
    return type;
}

// Gets the growth speed, this is how much the state is moved ahead by whenever the crop is watered
int Tomato::getGrowthSpeed() {
    return growthSpeed;
}

// Sets the growth speed, this will only be used for balancing the game
void Tomato::setGrowthSpeed(int speed){
    growthSpeed = speed;
}

// Setter to set when a crop is ready to be harvested
void Tomato::setHarvestable (bool ynHarvest) {
    harvestable = ynHarvest;
}

// Checks whether or not the crop is ready to be harvested
bool Tomato::getHarvestable () {
    return harvestable;
}

// Getter for state
int Tomato::getState() {
    return state;
}

// Destructor
Tomato::~Tomato() {
}
