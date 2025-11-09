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
                     const QString& plantPhase3TexPath)
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

    // Generic plant textures (fallback)
    plantPhase1Tex.load(plantPhase1TexPath);
    plantPhase2Tex.load(plantPhase2TexPath);
    plantPhase3Tex.load(plantPhase3TexPath);

    // Wheat-specific textures
    if (!wheatStage1Tex.load(":/assets/assets/Wheat_Stage_1.png")) {
        std::cerr << "Failed to load wheat stage 1" << std::endl;
        wheatStage1Tex = plantPhase1Tex; // Fallback
    }
    if (!wheatStage2Tex.load(":/assets/assets/Wheat_Stage_2.png")) {
        std::cerr << "Failed to load wheat stage 2" << std::endl;
        wheatStage2Tex = plantPhase2Tex; // Fallback
    }
    if (!wheatStage3Tex.load(":/assets/assets/Wheat.png")) {
        std::cerr << "Failed to load wheat stage 3" << std::endl;
        wheatStage3Tex = plantPhase3Tex; // Fallback
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

            // Draw base tile texture
            if (tile->getType() == "Field") {
                if (tile->getState() == 0) {
                    painter.drawPixmap(screenX, screenY, tileSize, tileSize, grassTex);
                } else {
                    painter.drawPixmap(screenX, screenY, tileSize, tileSize, dirtTex);
                }
            } else {
                painter.drawPixmap(screenX, screenY, tileSize, tileSize, grassTex);
            }

            // Draw crop if present
            if (tile->hasCrop()) {
                ICrops* crop = tile->getCrop();
                if (crop) {
                    int cropState = crop->getState();
                    std::string cropType = crop->getType();

                    // Render based on crop type and state
                    if (cropType == "Wheat") {
                        // Wheat: 0 (seed), 1, 2, 3 (harvestable)
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
                        // Tomato: 0 (seed), 2 (stage 1), 4 (harvestable) - grows by 2!
                        if (cropState == 0) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, seedTex);
                        } else if (cropState == 2) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, tomatoStage1Tex);
                        } else if (cropState >= 4) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, tomatoStage3Tex);
                        }
                    }
                    else if (cropType == "Corn") {
                        // Corn: 0 (seed), 1, 2, 3, 4 (harvestable)
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
                    else {
                        // Fallback for unknown crop types
                        if (cropState == 0) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, seedTex);
                        } else if (cropState == 1) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, plantPhase1Tex);
                        } else if (cropState == 2) {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, plantPhase2Tex);
                        } else {
                            painter.drawPixmap(screenX, screenY, tileSize, tileSize, plantPhase3Tex);
                        }
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
        std::cout << "Interaction failed: out of bounds or no player" << std::endl;
        return;
    }

    ITiles* tile = tiles[y][x].get();
    if (!tile) {
        std::cout << "Interaction failed: no tile at (" << x << ", " << y << ")" << std::endl;
        return;
    }

    ICrops* crop = tile->getCrop();
    IAnimal* animal = nullptr;

    ItemTypeEnum selectedItem = player->getSelectedItem();
    std::string tileType = tile->getType();
    int tileState = tile->getState();

    std::cout << "\n=== INTERACT DEBUG ===" << std::endl;
    std::cout << "Tile: (" << x << ", " << y << ")" << std::endl;
    std::cout << "Tile type BEFORE: " << tileType << std::endl;
    std::cout << "Tile state BEFORE: " << tileState << std::endl;
    std::cout << "Selected item: " << static_cast<int>(selectedItem) << std::endl;
    std::cout << "Has crop: " << (crop != nullptr) << std::endl;
    if (crop) {
        std::cout << "Crop type: " << crop->getType() << std::endl;
        std::cout << "Crop state: " << crop->getState() << std::endl;
        std::cout << "Crop harvestable: " << crop->getHarvestable() << std::endl;
    }

    std::unique_ptr<IInteract> interaction;

    // PRIORITY 1: Check for harvestable crops first (with empty hands or any tool)
    if (crop && crop->getHarvestable()) {
        std::cout << "Creating HarvestCropInteract (harvestable crop detected)" << std::endl;
        interaction = std::make_unique<HarvestCropInteraction>();
    }
    // PRIORITY 2: Tool-based interactions
    else if (selectedItem == ItemTypeEnum::HOE) {
        std::cout << "Creating TillingInteract" << std::endl;
        interaction = std::make_unique<TillingInteraction>();
    } else if (selectedItem == ItemTypeEnum::WATERING_CAN) {
        std::cout << "Creating WateringInteract" << std::endl;
        interaction = std::make_unique<WateringInteraction>();
    } else if (selectedItem == ItemTypeEnum::WHEAT_SEED ||
               selectedItem == ItemTypeEnum::TOMATO_SEED ||
               selectedItem == ItemTypeEnum::CORN_SEED) {
        std::cout << "Creating PlantingInteract" << std::endl;
        interaction = std::make_unique<PlantingInteraction>();
    }
    // PRIORITY 3: Animal interactions
    else if (animal && animal->getHarvestable()) {
        std::cout << "Creating HarvestAnimalInteract" << std::endl;
        interaction = std::make_unique<HarvestAnimalInteraction>();
    } else if (player->getItemCount(ItemTypeEnum::FEED) > 0 && animal) {
        std::cout << "Creating FeedAnimalInteract" << std::endl;
        interaction = std::make_unique<FeedAnimalInteraction>();
    }

    // Execute interaction if valid
    if (interaction) {
        std::cout << "Interact created successfully" << std::endl;
        bool isEquipped = interaction->isEquipped(player);
        std::cout << "Is equipped check: " << (isEquipped ? "PASSED" : "FAILED") << std::endl;

        if (isEquipped) {
            bool stateCheck = interaction->checkState(tile, crop, animal);
            std::cout << "State check: " << (stateCheck ? "PASSED" : "FAILED") << std::endl;

            if (stateCheck) {
                std::cout << "Executing action..." << std::endl;

                ITiles* currentTile = tiles[y][x].get();
                bool result = interaction->doAction(currentTile, crop, animal, player);

                std::cout << "Action result: " << (result ? "SUCCESS" : "FAILED") << std::endl;

                if (result) {
                    ITiles* updatedTile = tiles[y][x].get();

                    std::cout << "Tile state AFTER doAction: " << updatedTile->getState() << std::endl;
                    std::cout << "Tile type AFTER doAction: " << updatedTile->getType() << std::endl;

                    // Replace Yard with Field after tilling
                    if (updatedTile->getType() == "Yard" && updatedTile->getState() == 1) {
                        std::cout << "*** CONVERTING YARD TO FIELD ***" << std::endl;
                        std::unique_ptr<Field> newField = std::make_unique<Field>();
                        newField->setState(1);
                        tiles[y][x] = std::move(newField);
                        std::cout << "Tile type AFTER conversion: " << tiles[y][x]->getType() << std::endl;
                        std::cout << "Tile state AFTER conversion: " << tiles[y][x]->getState() << std::endl;
                    }
                }
            }
        }
    } else {
        std::cout << "No interact created for item: " << static_cast<int>(selectedItem) << std::endl;
    }
    std::cout << "=== END INTERACT DEBUG ===\n" << std::endl;
}

void TileBoard::advanceGrowth()
{
    for (int y = 0; y < boardHeight; ++y) {
        for (int x = 0; x < boardWidth; ++x) {
            ITiles* tile = tiles[y][x].get();
            if (!tile) continue;

            // Only grow crops if they are watered (as per requirements)
            if (tile->hasCrop() && tile->getWatered()) {
                ICrops* crop = tile->getCrop();
                if (crop) {
                    crop->setState(); // Advance growth
                }
            }

            // Reset watered state after growth (new day - crops need to be watered again)
            if (tile->getType() == "Field") {
                tile->setWatered(false);
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
