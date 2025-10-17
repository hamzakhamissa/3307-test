#include "icrops.h"
#include "string"

ICrops::ICrops() {

    class Crops{
    private:
        std::string type;
        int state;
        int grownState;
        int growthSpeed;
        bool harvestable;
    public:
        virtual int getState() = 0;
        virtual void setState() = 0;
        virtual int getGrownState() = 0;
        virtual std::string getType() = 0;
        virtual int getGrowthSpeed() = 0;
        virtual void setGrowthSpeed(int speed) = 0;
        virtual void setHarvestable (bool ynHarvest) = 0;
        virtual bool getHarvestable () = 0;
        virtual ~Crops(){}
    };

    class Corn : public Crops{

        std::string type;
        int state;
        int grownState;
        int growthSpeed;
        bool harvestable;

    private:

        int getState() override{
            return state;
        }

        int getGrownState() override{
            return grownState;
        }

        // Adds the growth speed to the state, when the state is equal to the grownState, the crop is ready to be harvested
        void setState() override{
            if (getState() < getGrownState()){
                state += growthSpeed;
            }

            if (getState() >= getGrownState()){
                setHarvestable(true);
            }
        }

        std::string getType() override{
            return type;
        }

        int getGrowthSpeed() override {
            return growthSpeed;
        }

        void setGrouthSpeed(int speed){
            growthSpeed = speed;
        }

        void setHarvestable (bool ynHarvest) override {
            harvestable = ynHarvest;
        }

        bool getHarvestable () override {
            return harvestable;
        }

    };

    class Tomato : public Crops {
        std::string type;
        int state;
        int grownState;
        int growthSpeed;
        bool harvestable;

    private:

        int getState() override{
            return state;
        }

        int getGrownState() override{
            return grownState;
        }

        // Adds the growth speed to the state, when the state is equal to the grownState, the crop is ready to be harvested
        void setState() override{
            if (getState() < getGrownState()){
                state += growthSpeed;
            }

            if (getState() >= getGrownState()){
                setHarvestable(true);
            }
        }

        std::string getType() override{
            return type;
        }

        int getGrowthSpeed() override {
            return growthSpeed;
        }

        void setGrouthSpeed(int speed){
            growthSpeed = speed;
        }

        void setHarvestable (bool ynHarvest) override {
            harvestable = ynHarvest;
        }

        bool getHarvestable () override {
            return harvestable;
        }

    };

    /*
     * Wheat object, subclass of Crops
     */
    class Wheat : public Crops {
        std::string type;
        int state;
        int grownState;
        int growthSpeed;
        bool harvestable;

    private:

        int getState() override{
            return state;
        }

        int getGrownState() override{
            return grownState;
        }

        // Adds the growth speed to the state, when the state is equal to the grownState, the crop is ready to be harvested
        void setState() override{
            if (getState() < getGrownState()){
                state += growthSpeed;
            }

            if (getState() >= getGrownState()){
                setHarvestable(true);
            }
        }

        std::string getType() override{
            return type;
        }

        int getGrowthSpeed() override {
            return growthSpeed;
        }

        void setGrouthSpeed(int speed){
            growthSpeed = speed;
        }

        void setHarvestable (bool ynHarvest) override {
            harvestable = ynHarvest;
        }

        bool getHarvestable () override {
            return harvestable;
        }

    };
}
