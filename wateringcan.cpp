#include "wateringcan.h"

// Constructor for the watering can class, sets appropriate attributes
WateringCan::WateringCan() {
    type = "Watering Can";
    state = 1;
}

// Gets the type of tool, should always be watering can
std::string WateringCan::getType() {
    return type;
}

// Gets the state of the tool
int WateringCan::getState() {
    return state;
}

// Sets the state of the tool
void WateringCan::setState(int newState) {
    state = newState;
}

// Destructor
WateringCan::~WateringCan() {
}
