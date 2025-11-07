#include "character.h"

Character::Character(QWidget* parent) : QLabel(parent) {
    xPosition = 400.0f;
    yPosition = 300.0f;
    speed = 10;
    state = 0;
    equiped = false;
    money = 50;
    //cropsInventory = {};
    //toolInventory = {};
    setPixmap(QPixmap(":/assets/assets/Idle_character.png"));
    move(100, 100);
}

int Character::getState(){
    return state;
}

void Character::setState(int newState){
    state = newState;
}

int Character::getX(){
    return xPosition;
}

void Character::setX(int newX){
    xPosition = newX;
}

int Character::getY(){
    return yPosition;
}

void Character::setY(int newY){
    yPosition = newY;
}

int Character::getSpeed(){
    return speed;
}

void Character::setSpeed(int newSpeed){
    speed = newSpeed;
}

int Character::getMoney(){
    return money;
}

void Character::setMoney(int newMoney){
    money = newMoney;
}

void Character::moveRight(){
    move(x() + speed, y());
}

void Character::moveLeft(){
    move(x() - speed, y());
}

void Character::moveUp(){
    move(x(), y() - speed);
}

void Character::moveDown(){
    move(x(), y() + speed);
}

bool Character::isEquiped(){
    return equiped;
}

void Character::sleep(){

}

Character::~Character(){

}
