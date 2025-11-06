#include "water.h"

/* Constructor for the water class, sets appropriate attributes, acts as an obsatcle,

all attributes should stay the same.
*/

Water::Water() {
    state = 0;
    type = "Water";
    crops = nullptr;
    ownsCrop = false;
    watered = false;
}

// Gets the state of the itile object
int Water::getState(){
    return state;
}

// Sets new state for the itile object
void Water::setState(int newstate){
    state = newstate;
}

// Gets the type of the itile object, should always be water
std::string Water::getType(){
    return type;
}

// Sets type of itile object, shouldn't have to be used
void Water::setType(std::string newType){
    type = newType;
}

// Gets the current crop that the itile object holds
ICrops* Water::getCrop(){
    return crops;
}

// Sets the crop that the itile object will hold
void Water::setCrop(ICrops* crop){
    crops = crop;
}

// Checks if the itile object has a crop
bool Water::hasCrop(){
    return ownsCrop;
}

// Checks if the itile object was watered
bool Water::getWatered(){
    return watered;
}

// Sets the itile object to be watered when the watering can is used on it, or to false when the day ends
void Water::setWatered(bool newWatered){
    watered = newWatered;
}
