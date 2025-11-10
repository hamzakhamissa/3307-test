#ifndef IINTERACT_H
#define IINTERACT_H

// Forward declarations
class ITiles;
class ICrops;
class IAnimal;
class Player;

// Interface for interaction behaviors
class IInteract {
public:
    IInteract() = default;
    virtual ~IInteract() = default;

    virtual bool doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) = 0;
    virtual bool checkState(ITiles* tile, ICrops* crop, IAnimal* animal) = 0;
    virtual bool isEquipped(Player* player) = 0;
};

// Tilling interaction
class TillingInteraction : public IInteract {
public:
    bool doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) override;
    bool checkState(ITiles* tile, ICrops* crop, IAnimal* animal) override;
    bool isEquipped(Player* player) override;

    ITiles* createReplacementTile(ITiles* oldTile);
};

// Planting interaction
class PlantingInteraction : public IInteract {
public:
    bool doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) override;
    bool checkState(ITiles* tile, ICrops* crop, IAnimal* animal) override;
    bool isEquipped(Player* player) override;
};

// Watering interaction
class WateringInteraction : public IInteract {
public:
    bool doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) override;
    bool checkState(ITiles* tile, ICrops* crop, IAnimal* animal) override;
    bool isEquipped(Player* player) override;
};

// Harvest Crop interaction
class HarvestCropInteraction : public IInteract {
public:
    bool doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) override;
    bool checkState(ITiles* tile, ICrops* crop, IAnimal* animal) override;
    bool isEquipped(Player* player) override;
};

// Harvest Animal interaction
class HarvestAnimalInteraction : public IInteract {
public:
    bool doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) override;
    bool checkState(ITiles* tile, ICrops* crop, IAnimal* animal) override;
    bool isEquipped(Player* player) override;
};

// Feed Animal interaction
class FeedAnimalInteraction : public IInteract {
public:
    bool doAction(ITiles* tile, ICrops* crop, IAnimal* animal, Player* player) override;
    bool checkState(ITiles* tile, ICrops* crop, IAnimal* animal) override;
    bool isEquipped(Player* player) override;
};

#endif // IINTERACT_H

