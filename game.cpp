#include "game.h"
#include "itemtypeenum.h"
#include "inventory.h"
#include <iostream>

Game::Game(QWidget *parent)
    : QWidget(parent)
    , gameTimer(nullptr)
    , running(false)
    , shopOpen(false)
    , menuOpen(false)
    , shopBuyMode(true)
    , dayNumber(1)
    , tiles(50, 50, TILE_SIZE)   // map dims here
    , cameraX(0.0f)
    , cameraY(0.0f)
    , mouseLeftPressed(false)
    , mouseLeftPressedLastFrame(false)
    , elapsedTimer(nullptr)
{
    // Create player
    player = std::make_unique<Player>();
    player->setPosition(400.0f, 300.0f);
    player->setSpeed(100.0f);

    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowTitle("Farmland - Farming Simulator");
    setFocusPolicy(Qt::StrongFocus);
}

Game::~Game() {}

bool Game::init()
{
    // Character & held-item textures
    charDownTex.load(":/assets/assets/Down_character.png");
    charLeftTex.load(":/assets/assets/Left_character.png");
    charRightTex.load(":/assets/assets/Right_character.png");
    charUpTex.load(":/assets/assets/Up_character.png");
    charIdleTex.load(":/assets/assets/Idle_character.png");
    hoeTex.load(":/assets/assets/hoe.png");
    seedTex.load(":/assets/assets/Seed_1.png");
    wateringCanTex.load(":/assets/assets/Watering_Can.png");
    
    // Seed textures
    wheatSeedTex.load(":/assets/assets/Wheat_seed.png");
    tomatoSeedTex.load(":/assets/assets/Tomato_seed.png");
    cornSeedTex.load(":/assets/assets/Corn_seed.png");
    
    // Crop textures
    wheatTex.load(":/assets/assets/Wheat.png");
    tomatoTex.load(":/assets/assets/Tomato.png");
    cornTex.load(":/assets/assets/Corn.png");
    
    // Animal product textures (using placeholder for now - may need to add to resources)
    // milkTex and eggTex will be handled separately if needed
    
    // Animal textures (for shop display)
    cowTex.load(":/assets/assets/Cow_right.png");
    chickenTex.load(":/assets/assets/Chicken_right.png");

    // Tileboard textures (board owns them)
    if (!tiles.init(
            ":/assets/assets/tile.png",               // grass
            ":/assets/assets/dirt.png",               // dirt
            ":/assets/assets/Seed_1.png",             // seed overlay
            ":/assets/assets/Plant_1_Phase_1.png",
            ":/assets/assets/Plant_1_Phase_2.png",
            ":/assets/assets/Plant_1_Phase_3.png")) {
        std::cerr << "Tileboard textures failed to load!\n";
        return false;
    }

    // Timer
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &Game::gameLoop);
    gameTimer->start(16);

    // Timing
    elapsedTimer = new QElapsedTimer();
    elapsedTimer->start();

    running = true;
    return true;
}

void Game::start()
{
    show();
    init();
}

void Game::gameLoop()
{
    if (!running) return;

    static qint64 lastTime = -1;
    qint64 currentTime = elapsedTimer->elapsed();
    float dt;

    if (lastTime < 0) {
        dt = 0.016f;
        lastTime = currentTime;
    } else {
        dt = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        if (dt > 0.1f) dt = 0.1f;
    }

    handleInput();
    update(dt);
    repaint();
}

void Game::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(135, 206, 235));
    render(p);
}

void Game::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    keys[key] = true;

    if (key == Qt::Key_Escape) {
        if (shopOpen) {
            closeShop();
        } else if (menuOpen) {
            closeMenu();
        } else {
            openMenu();
        }
    }
    if (key == Qt::Key_T) { advanceGrowthCycle(); } // Temporary - will be replaced with sleep
    
    // Menu interactions (check first so they take priority)
    if (menuOpen) {
        if (key == Qt::Key_1) {
            saveGame(1);
            closeMenu();
        } else if (key == Qt::Key_2) {
            loadGame(1);
            closeMenu();
        } else if (key == Qt::Key_3) {
            closeMenu();
            // Could exit game here if needed
        }
    }
    // Shop interactions (check before inventory selection)
    else if (shopOpen) {
        // Toggle between buy and sell mode with Tab key
        if (key == Qt::Key_Tab) {
            shopBuyMode = !shopBuyMode;
        }
        
        if (shopBuyMode) {
            // Buy mode - keys 1-9 to buy items
            auto buyableItems = market.getBuyableItems();
            if (key >= Qt::Key_1 && key <= Qt::Key_9) {
                int index = key - Qt::Key_1;
                if (index < static_cast<int>(buyableItems.size())) {
                    market.buy(player.get(), buyableItems[index].itemType, 1);
                }
            }
        } else {
            // Sell mode - keys 1-9 to sell items
            auto sellableItems = market.getSellableItems(player.get());
            if (key >= Qt::Key_1 && key <= Qt::Key_9) {
                int index = key - Qt::Key_1;
                if (index < static_cast<int>(sellableItems.size())) {
                    market.sell(player.get(), sellableItems[index].itemType, 1);
                }
            }
        }
    }
    // Normal game controls (only when shop/menu not open)
    else {
        // Inventory slot selection (1-9)
        if (key >= Qt::Key_1 && key <= Qt::Key_9) {
            player->setSelectedSlot(key - Qt::Key_1);
        }
        if (key == Qt::Key_E) { interactWithFrontTile(); }
        if (key == Qt::Key_M) {
            openShop();
        }
    }
}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    keys[event->key()] = false;
}

void Game::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mouseLeftPressed = true;
        interactWithFrontTile();
    }
}

void Game::handleInput()
{
    mouseLeftPressedLastFrame = mouseLeftPressed;
    mouseLeftPressed = false;
}

void Game::getTileAtPlayer(int &tileX, int &tileY)
{
    player->getFrontTileIndices(TILE_SIZE, CHARACTER_SIZE, tileX, tileY);
}

void Game::interactWithFrontTile()
{
    if (shopOpen || menuOpen) return; // Don't interact when UI is open

    int tx, ty;
    getTileAtPlayer(tx, ty);
    
    // Debug output
    std::cout << "Interacting at tile (" << tx << ", " << ty << ")" << std::endl;
    std::cout << "Selected item: " << static_cast<int>(player->getSelectedItem()) << std::endl;
    std::cout << "Selected slot: " << player->getSelectedSlot() << std::endl;
    
    if (tiles.inBounds(tx, ty)) {
        ITiles* tile = tiles.getTile(tx, ty);
        if (tile) {
            std::cout << "Tile type: " << tile->getType() << ", State: " << tile->getState() << std::endl;
        }
        tiles.interactAt(tx, ty, player.get());
    } else {
        std::cout << "Tile out of bounds!" << std::endl;
    }
}

void Game::advanceGrowthCycle()
{
    tiles.advanceGrowth();
}

void Game::update(float deltaTime)
{
    if (!shopOpen && !menuOpen) {
        player->updateFromKeys(keys, deltaTime);
        player->clampToMap(tiles.width(), tiles.height(), TILE_SIZE, CHARACTER_SIZE);
    }

    // Camera follow
    cameraX = player->getX() + CHARACTER_SIZE / 2.0f - WINDOW_WIDTH / 2.0f;
    cameraY = player->getY() + CHARACTER_SIZE / 2.0f - WINDOW_HEIGHT / 2.0f;

    // Clamp camera
    const float maxCamX = tiles.width()  * TILE_SIZE - WINDOW_WIDTH;
    const float maxCamY = tiles.height() * TILE_SIZE - WINDOW_HEIGHT;
    if (cameraX < 0) cameraX = 0; else if (cameraX > maxCamX) cameraX = maxCamX;
    if (cameraY < 0) cameraY = 0; else if (cameraY > maxCamY) cameraY = maxCamY;
}

void Game::render(QPainter &painter)
{
    // Tiles (board draws everything)
    tiles.render(painter, cameraX, cameraY, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Draw tile indicator (before character so it appears underneath)
    if (!shopOpen && !menuOpen) {
        renderTileIndicator(painter);
    }

    // Character (sprite + held item)
    ItemTypeEnum selectedItemEnum = player->getSelectedItem();
    ItemType selectedItem = player->convertItemTypeEnumToItemType(selectedItemEnum);

    player->renderCharacter(painter,
                            charDownTex, charLeftTex, charRightTex, charUpTex, charIdleTex,
                            cameraX, cameraY, CHARACTER_SIZE);
    player->renderHeldItem(painter, selectedItem, hoeTex, seedTex, wateringCanTex,
                           cameraX, cameraY, CHARACTER_SIZE);

    // Render UI overlays
    renderInventoryUI(painter);

    if (shopOpen) {
        renderShopUI(painter);
    }

    if (menuOpen) {
        renderMenuUI(painter);
    }
}

void Game::openShop() {
    shopOpen = true;
}

void Game::closeShop() {
    shopOpen = false;
}

void Game::openMenu() {
    menuOpen = true;
}

void Game::closeMenu() {
    menuOpen = false;
}

void Game::saveGame(int saveSlot) {
    QString savePath = Menu::getDefaultSavePath(saveSlot);
    menu.save(savePath, player.get(), &tiles, dayNumber);
}

void Game::loadGame(int saveSlot) {
    QString savePath = Menu::getDefaultSavePath(saveSlot);
    if (Menu::saveExists(saveSlot)) {
        menu.load(savePath, player.get(), &tiles, dayNumber);
    }
}

void Game::renderInventoryUI(QPainter& painter)
{
    // Draw inventory bar at bottom of screen
    const int invBarY = WINDOW_HEIGHT - 80;
    const int slotSize = 60;
    const int slotSpacing = 5;
    const int startX = (WINDOW_WIDTH - (Inventory::INVENTORY_SIZE * (slotSize + slotSpacing) - slotSpacing)) / 2;

    // Background
    painter.fillRect(0, invBarY, WINDOW_WIDTH, 80, QColor(50, 50, 50, 200));

    Inventory* inv = player->getInventory();
    if (!inv) return;

    // Changed 'slots' to 'invSlots' to avoid Qt macro conflict
    const std::vector<InventorySlot>& invSlots = inv->getSlots();
    int selectedSlot = player->getSelectedSlot();

    for (int i = 0; i < Inventory::INVENTORY_SIZE && i < static_cast<int>(invSlots.size()); ++i) {
        int x = startX + i * (slotSize + slotSpacing);
        int y = invBarY + 10;

        // Draw slot background
        QColor slotColor = (i == selectedSlot) ? QColor(255, 255, 0, 150) : QColor(100, 100, 100, 150);
        painter.fillRect(x, y, slotSize, slotSize, slotColor);
        painter.setPen(Qt::white);
        painter.drawRect(x, y, slotSize, slotSize);

        // Draw item sprite and info
        if (invSlots[i].itemType != ItemTypeEnum::NONE && invSlots[i].quantity > 0) {
            // Get sprite for this item
            const QPixmap* itemSprite = getItemSprite(invSlots[i].itemType);
            if (itemSprite && !itemSprite->isNull()) {
                painter.drawPixmap(x + 2, y + 2, slotSize - 4, slotSize - 4, *itemSprite);
            }
        }
        
        // Draw slot number (1-9) in bottom right corner
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 12, QFont::Bold));
        painter.drawText(x + slotSize - 18, y + slotSize - 5, QString::number(i + 1));
    }

    // Draw money
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(10, invBarY + 30, QString("Money: $%1").arg(player->getMoney()));
}

void Game::renderShopUI(QPainter& painter)
{
    // Draw shop overlay
    painter.fillRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, QColor(0, 0, 0, 200));
    
    const int shopWidth = 600;
    const int shopHeight = 500;
    const int shopX = (WINDOW_WIDTH - shopWidth) / 2;
    const int shopY = (WINDOW_HEIGHT - shopHeight) / 2;
    
    // Shop background
    painter.fillRect(shopX, shopY, shopWidth, shopHeight, QColor(100, 80, 60));
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.drawText(shopX + 20, shopY + 30, "SHOP");
    
    // Mode indicator
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    if (shopBuyMode) {
        painter.setPen(QColor(200, 255, 200));
        painter.drawText(shopX + shopWidth - 150, shopY + 30, "[BUY MODE]");
    } else {
        painter.setPen(QColor(255, 200, 200));
        painter.drawText(shopX + shopWidth - 150, shopY + 30, "[SELL MODE]");
    }
    painter.setPen(Qt::white);
    
    const int itemHeight = 40;
    const int iconSize = 35;
    int yPos = shopY + 70;
    painter.setFont(QFont("Arial", 12));
    
    if (shopBuyMode) {
        // Draw Buy section
        auto buyableItems = market.getBuyableItems();
        
        for (size_t i = 0; i < buyableItems.size() && i < 9; ++i) {
            const auto& item = buyableItems[i];
            int itemY = yPos + static_cast<int>(i) * itemHeight;
            
            // Draw item icon
            const QPixmap* itemSprite = getItemSprite(item.itemType);
            if (itemSprite && !itemSprite->isNull()) {
                painter.drawPixmap(shopX + 30, itemY + 2, iconSize, iconSize, *itemSprite);
            }
            
            // Draw item name and price
            QString itemText = QString("%1 - $%2").arg(QString::fromStdString(item.name)).arg(item.buyPrice);
            painter.drawText(shopX + 75, itemY + 25, itemText);
            
            // Draw buy button indicator
            painter.setPen(QColor(200, 255, 200));
            painter.drawText(shopX + 350, itemY + 25, QString("[%1]").arg(i + 1));
            painter.setPen(Qt::white);
        }
    } else {
        // Draw Sell section
        auto sellableItems = market.getSellableItems(player.get());
        
        for (size_t i = 0; i < sellableItems.size() && i < 9; ++i) {
            const auto& item = sellableItems[i];
            int itemY = yPos + static_cast<int>(i) * itemHeight;
            
            // Draw item icon - use sprites for all items
            const QPixmap* itemSprite = getItemSprite(item.itemType);
            if (itemSprite && !itemSprite->isNull()) {
                painter.drawPixmap(shopX + 30, itemY + 2, iconSize, iconSize, *itemSprite);
            }
            
            // Draw item name, price, and quantity
            int quantity = player->getItemCount(item.itemType);
            QString itemText = QString("%1 - $%2 (x%3)").arg(QString::fromStdString(item.name))
                              .arg(item.sellPrice).arg(quantity);
            painter.drawText(shopX + 75, itemY + 25, itemText);
            
            // Draw sell button indicator
            painter.setPen(QColor(255, 200, 200));
            painter.drawText(shopX + 400, itemY + 25, QString("[%1]").arg(i + 1));
            painter.setPen(Qt::white);
        }
    }
    
    // Instructions
    painter.setFont(QFont("Arial", 10));
    painter.drawText(shopX + 20, shopY + shopHeight - 60, "Press TAB to switch between Buy/Sell");
    painter.drawText(shopX + 20, shopY + shopHeight - 40, "Press 1-9 to buy/sell items");
    painter.drawText(shopX + 20, shopY + shopHeight - 20, "Press M or ESC to close shop");
}

const QPixmap* Game::getItemSprite(ItemTypeEnum itemType) const {
    switch (itemType) {
        case ItemTypeEnum::HOE: return &hoeTex;
        case ItemTypeEnum::WATERING_CAN: return &wateringCanTex;
        case ItemTypeEnum::WHEAT_SEED: return &wheatSeedTex;
        case ItemTypeEnum::TOMATO_SEED: return &tomatoSeedTex;
        case ItemTypeEnum::CORN_SEED: return &cornSeedTex;
        case ItemTypeEnum::WHEAT: return &wheatTex;
        case ItemTypeEnum::TOMATO: return &tomatoTex;
        case ItemTypeEnum::CORN: return &cornTex;
        case ItemTypeEnum::COW: return &cowTex;
        case ItemTypeEnum::CHICKEN: return &chickenTex;
        default: return nullptr;
    }
}

void Game::renderMenuUI(QPainter& painter)
{
    // Draw menu overlay
    painter.fillRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, QColor(0, 0, 0, 200));
    
    const int menuWidth = 400;
    const int menuHeight = 300;
    const int menuX = (WINDOW_WIDTH - menuWidth) / 2;
    const int menuY = (WINDOW_HEIGHT - menuHeight) / 2;
    
    // Menu background
    painter.fillRect(menuX, menuY, menuWidth, menuHeight, QColor(60, 60, 80));
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.drawText(menuX + 20, menuY + 30, "MENU");
    
    // Menu options
    painter.setFont(QFont("Arial", 14));
    int yPos = menuY + 70;
    painter.drawText(menuX + 20, yPos, "1. Save Game");
    yPos += 30;
    painter.drawText(menuX + 20, yPos, "2. Load Game");
    yPos += 30;
    painter.drawText(menuX + 20, yPos, "3. Exit");
    
    // Instructions
    painter.setFont(QFont("Arial", 10));
    painter.drawText(menuX + 20, menuY + menuHeight - 40, "Press 1-3 to select, ESC to close");
}

void Game::renderTileIndicator(QPainter& painter)
{
    // Get the tile the player is facing
    int tileX, tileY;
    getTileAtPlayer(tileX, tileY);

    // Check if tile is valid
    if (!tiles.inBounds(tileX, tileY)) return;

    // Calculate screen position of the tile
    int screenX = tileX * TILE_SIZE - static_cast<int>(cameraX);
    int screenY = tileY * TILE_SIZE - static_cast<int>(cameraY);

    // Draw a colored border around the target tile
    painter.setPen(QPen(QColor(255, 255, 0, 200), 3)); // Yellow border, 3px thick
    painter.setBrush(Qt::NoBrush); // No fill
    painter.drawRect(screenX, screenY, TILE_SIZE, TILE_SIZE);

    // Optional: Draw a semi-transparent overlay
    QColor overlayColor(255, 255, 0, 50); // Yellow with 50 alpha
    painter.fillRect(screenX, screenY, TILE_SIZE, TILE_SIZE, overlayColor);
}
