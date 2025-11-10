#ifndef ITOOL_H
#define ITOOL_H
#include <string>

class ITool
{
public:
    ITool();
    ~ITool();

    virtual int getState() = 0;
    virtual void setState(int newState) = 0;
    virtual std::string getType() = 0;

protected:
    std::string type;
    int state;
};

#endif // ITOOL_H
