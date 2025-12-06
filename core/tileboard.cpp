#include "tileboard.h"
#include "player.h"
#include "iinteract.h"
#include "itemtypeenum.h"
#include "ianimal.h"
#include "field.h"
#include "wheat.h"
#include <iostream>

TileBoard::TileBoard(int width, int height, int tileSize)
    : boardWidth(width)
    , boardHeight(height)
    , tileSize(tileSize)
{
    tiles.resize(boardHeight);
    for (int y = 0; y < boardHeight; ++y) {
        tiles[y].resize(boardWidth);
        for (int x = 0; x < boardWidth; ++x) {
            // Create a mix of Yard and Field tiles
            // Center area (20x20) is Field tiles for farming
            if (x >= 15 && x < 35 && y >= 15 && y < 35) {
                tiles[y][x] = std::make_unique<Field>();
            } else {
                tiles[y][x] = std::make_unique<Yard>();
            }
        }
    }
}

TileBoard::~TileBoard() = default;

bool TileBoard::init(const QString& grassTexPath,
                     const QString& dirtTexPath,
                     const QString& seedTexPath,
                     const QString& plantPhase1TexPath,
                     const QString& plantPhase2TexPath,
                     const QString& plantPhase3TexPath,
                     const QString& cowTexPath,
                     const QString& chickenTexPath)
{
    bool success = true;

    // Base textures
    if (!grassTex.load(grassTexPath)) {
        std::cerr << "Failed to load grass texture" << std::endl;
        success = false;
    }
    if (!dirtTex.load(dirtTexPath)) {
        std::cerr << "Failed to load dirt texture" << std::endl;
        success = false;
    }
    if (!seedTex.load(seedTexPath)) {
        std::cerr << "Failed to load seed texture" << std::endl;
        success = false;
    }

    plantPhase1Tex.load(plantPhase1TexPath);
    plantPhase2Tex.load(plantPhase2TexPath);
    plantPhase3Tex.load(plantPhase3TexPath);

    // Wheat-specific textures
    if (!wheatStage1Tex.load(":/assets/assets/Wheat_Stage_1.png")) {
        std::cerr << "Failed to load wheat stage 1" << std::endl;
        wheatStage1Tex = plantPhase1Tex;
    }
    if (!wheatStage2Tex.load(":/assets/assets/Wheat_Stage_2.png")) {
        std::cerr << "Failed to load wheat stage 2" << std::endl;
        wheatStage2Tex = plantPhase2Tex;
    }
    if (!wheatStage3Tex.load(":/assets/assets/Wheat.png")) {
        std::cerr << "Failed to load wheat stage 3" << std::endl;
        wheatStage3Tex = plantPhase3Tex;
    }

    // Tomato-specific textures
    if (!tomatoStage1Tex.load(":/assets/assets/Tomato_Stage_1.png")) {
        std::cerr << "Failed to load tomato stage 1" << std::endl;
        tomatoStage1Tex = plantPhase1Tex;
    }
    if (!tomatoStage2Tex.load(":/assets/assets/Tomato_Stage_2.png")) {
        std::cerr << "Failed to load tomato stage 2" << std::endl;
        tomatoStage2Tex = plantPhase2Tex;
    }
    if (!tomatoStage3Tex.load(":/assets/assets/Tomato.png")) {
        std::cerr << "Failed to load tomato stage 3" << std::endl;
        tomatoStage3Tex = plantPhase3Tex;
    }

    // Corn-specific textures
    if (!cornStage1Tex.load(":/assets/assets/Corn_Stage_1.png")) {
        std::cerr << "Failed to load corn stage 1" << std::endl;
        cornStage1Tex = plantPhase1Tex;
    }
    if (!cornStage2Tex.load(":/assets/assets/Corn_Stage_2.png")) {
        std::cerr << "Failed to load corn stage 2" << std::endl;
        cornStage2Tex = plantPhase2Tex;
    }
    if (!cornStage3Tex.load(":/assets/assets/Corn.png")) {
        std::cerr << "Failed to load corn stage 3" << std::endl;
        cornStage3Tex = plantPhase3Tex;
    }

    // Animal textures
    if (!cowTex.load(cowTexPath)) {
        std::cerr << "Failed to load cow texture" << std::endl;
        success = false;
    }
    if (!chickenTex.load(chickenTexPath)) {
        std::cerr << "Failed to load chicken texture" << std::endl;
        success = false;
    }

    return success;
}

void TileBoard::render(QPainter& painter, float cameraX, float cameraY, int viewWidth, int viewHeight)
{
    int startX = static_cast<int>(cameraX) / tileSize;
    int startY = static_cast<int>(cameraY) / tileSize;
    int endX = (static_cast<int>(cameraX) + viewWidth) / tileSize + 1;
    int endY = (static_cast<int>(cameraY) + viewHeight) / tileSize + 1;

    startX = (startX < 0) ? 0 : startX;
    startY = (startY < 0) ? 0 : startY;
    endX = (endX > boardWidth) ? boardWidth : endX;
    endY = (endY > boardHeight) ? boardHeight : endY;

    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            int screenX = x * tileSize - static_cast<int>(cameraX);
            int screenY = y * tileSize - static_cast<int>(cameraY);

            ITiles* tile = tiles[y][x].get();
            if (!tile) continue;

            if (tile->getType() == "Field") {
                if (tile->getState() == 0) {
                    painter.drawPixmap(screenX, screenY, tileSize, tileSize, grassTex);
                } else {
                    painter.drawPixmap(screenX, screenY, tileSize, tileSize, dirtTex);
                }
            } else {
                painter.drawPixmap(screenX, screenY, tileSize, tileSize, grassTex);
            }

            if (tile->hasCrop()) {
                ICrops* crop = tile->getCrop();
                if (crop) {
                    int cropState = crop->getState();
                    std::string cropType = crop->getType();

                    // Render based on crop type and state
                    if (cropType == "Wheat") {
                        if (cropState == 0) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, seedTex);
                        } else if (cropState == 1) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, wheatStage1Tex);
                        } else if (cropState == 2) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, wheatStage2Tex);
                        } else if (cropState >= 3) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, wheatStage3Tex);
                        }
                    }
                    else if (cropType == "Tomato") {
                        if (cropState == 0) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, seedTex);
                        } else if (cropState == 2) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, tomatoStage1Tex);
                        } else if (cropState >= 4) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, tomatoStage3Tex);
                        }
                    }
                    else if (cropType == "Corn") {
                        if (cropState == 0) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, seedTex);
                        } else if (cropState == 1) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, cornStage1Tex);
                        } else if (cropState == 2 || cropState == 3) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, cornStage2Tex);
                        } else if (cropState >= 4) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, cornStage3Tex);
                        }
                    }
                }
            }

            // Draw animal if present
            if (tile->hasAnimal()) {
                IAnimal* animal = tile->getAnimal();
                if (animal) {
                    std::string animalType = animal->getType();

                    if (animalType == "Cow") {
                        painter.drawPixmap(screenX, screenY, tileSize, tileSize, cowTex);
                    } else if (animalType == "Chicken") {
                        painter.drawPixmap(screenX, screenY, tileSize, tileSize, chickenTex);
                    }

                    // Draw indicator if animal is harvestable (fed and ready)
                    if (animal->getHarvestable()) {
                        QColor readyColor(0, 255, 0, 100); 
                        painter.fillRect(screenX, screenY, tileSize, tileSize, readyColor);
                    }
                }
            }

            // Draw watered indicator
            if (tile->getWatered() && tile->getType() == "Field") {
                QColor waterColor(100, 150, 255, 100);
                painter.fillRect(screenX, screenY, tileSize, tileSize, waterColor);
            }
        }
    }
}

void TileBoard::interactAt(int x, int y, Player* player)
{
    if (!inBounds(x, y) || !player) {
        return;
    }

    ITiles* tile = tiles[y][x].get();
    if (!tile) return;

    ICrops* crop = tile->getCrop();
    IAnimal* animal = tile->getAnimal();

    ItemTypeEnum selectedItem = player->getSelectedItem();

    std::unique_ptr<IInteract> interaction;

    if (crop && crop->getHarvestable()) {
        interaction = std::make_unique<HarvestCropInteraction>();
    } else if (animal && animal->getHarvestable()) {
        interaction = std::make_unique<HarvestAnimalInteraction>();
    }
    else if (selectedItem == ItemTypeEnum::COW || selectedItem == ItemTypeEnum::CHICKEN) {
        interaction = std::make_unique<PlaceAnimalInteraction>();
    }
    else if (animal && (player->getItemCount(ItemTypeEnum::WHEAT) > 0 ||
                        player->getItemCount(ItemTypeEnum::CORN) > 0 ||
                        player->getItemCount(ItemTypeEnum::FEED) > 0)) {
        interaction = std::make_unique<FeedAnimalInteraction>();
    }
    else if (selectedItem == ItemTypeEnum::HOE) {
        interaction = std::make_unique<TillingInteraction>();
    } else if (selectedItem == ItemTypeEnum::WATERING_CAN) {
        interaction = std::make_unique<WateringInteraction>();
    } else if (selectedItem == ItemTypeEnum::WHEAT_SEED ||
               selectedItem == ItemTypeEnum::TOMATO_SEED ||
               selectedItem == ItemTypeEnum::CORN_SEED) {
        interaction = std::make_unique<PlantingInteraction>();
    }

    if (interaction) {
        if (interaction->isEquipped(player) && interaction->checkState(tile, crop, animal)) {
            ITiles* currentTile = tiles[y][x].get();
            bool result = interaction->doAction(currentTile, crop, animal, player);

            if (result) {
                ITiles* updatedTile = tiles[y][x].get();

                if (updatedTile->getType() == "Yard" && updatedTile->getState() == 1) {
                    std::unique_ptr<Field> newField = std::make_unique<Field>();
                    newField->setState(1);
                    tiles[y][x] = std::move(newField);
                }
            }
        }
    }
}

void TileBoard::advanceGrowth()
{
    for (int y = 0; y < boardHeight; ++y) {
        for (int x = 0; x < boardWidth; ++x) {
            ITiles* tile = tiles[y][x].get();
            if (!tile) continue;

            // Only grow crops if they are watered
            if (tile->hasCrop() && tile->getWatered()) {
                ICrops* crop = tile->getCrop();
                if (crop) {
                    crop->setState();
                }
            }

            if (tile->getType() == "Field") {
                tile->setWatered(false);
            }

            if (tile->hasAnimal()) {
                IAnimal* animal = tile->getAnimal();
                if (animal && animal->getState() == 1) {
                } else {
                    if (animal) {
                        animal->setState(0);
                        animal->setHarvestable();
                    }
                }
            }
        }
    }
}

bool TileBoard::inBounds(int x, int y) const
{
    return x >= 0 && x < boardWidth && y >= 0 && y < boardHeight;
}

ITiles* TileBoard::getTile(int x, int y)
{
    if (!inBounds(x, y)) return nullptr;
    return tiles[y][x].get();
}

void TileBoard::setTile(int x, int y, ITiles* tile)
{
    if (!inBounds(x, y)) return;
    tiles[y][x].reset(tile);
}
