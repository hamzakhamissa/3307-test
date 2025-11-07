#ifndef CHARACTER_H
#define CHARACTER_H
#include <vector>

#include <QWidget>
#include <QLabel>

#include "icrops.h"
#include "itool.h"

class Character : public QLabel
{
    Q_OBJECT
public:
    Character(QWidget* parent = nullptr);
    ~Character();

    int getState();
    void setState(int newState);

    int getX();
    int getY();
    void setX(int newX);
    void setY(int newY);

    int getSpeed() ;
    void setSpeed(int newSpeed) ;

    //Need a fix to inventory, currently cannot hold animals
    std::vector<ICrops> getCropsInventory() ;
    std::vector<ITool> getToolInventory() ;
    void addCropsInventory(ICrops *crop) ;
    void addToolInventory(ITool *tool) ;

    void setMoney(int newMoney) ;
    int getMoney() ;

    void moveLeft() ;
    void moveRight() ;
    void moveUp() ;
    void moveDown() ;

    bool isEquiped() ;

    void interact() ;

    void sleep() ;

private:
    int xPosition;
    int yPosition;
    int speed;
    int state;
    bool equiped;
    int money;
    std::vector<ICrops> cropsInventory;
    std::vector<ITool> toolInventory;
};

#endif // CHARACTER_H
