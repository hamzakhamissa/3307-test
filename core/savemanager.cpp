#include "savemanager.h"
#include "player.h"
#include "tileboard.h"
#include "inventory.h"
#include "itiles.h"
#include "icrops.h"
#include "ianimal.h"
#include "cropfactory.h"
#include "animalfactory.h"
#include "field.h"
#include "yard.h"
#include <QDateTime>
#include <iostream>

// Singleton instance
SaveManager& SaveManager::getInstance() {
    static SaveManager instance;
    return instance;
}

SaveManager::SaveManager() {
    ensureSaveDirectoryExists();
}

SaveManager::~SaveManager() {}

QString SaveManager::getAppDataPath() const {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return path + "/saves";
}

void SaveManager::ensureSaveDirectoryExists() const {
    QString savePath = getAppDataPath();
    QDir dir;
    if (!dir.exists(savePath)) {
        dir.mkpath(savePath);
    }
}

QString SaveManager::getSavePath(int saveSlot) const {
    return getAppDataPath() + QString("/save_%1.json").arg(saveSlot);
}

bool SaveManager::saveExists(int saveSlot) const {
    return QFile::exists(getSavePath(saveSlot));
}

bool SaveManager::deleteSave(int saveSlot) {
    QString path = getSavePath(saveSlot);
    return QFile::remove(path);
}

SaveManager::SaveInfo SaveManager::getSaveInfo(int saveSlot) const {
    SaveInfo info;
    info.slot = saveSlot;
    info.exists = saveExists(saveSlot);

    if (!info.exists) {
        info.dayNumber = 0;
        info.money = 0;
        info.timestamp = "";
        return info;
    }

    QFile file(getSavePath(saveSlot));
    if (!file.open(QIODevice::ReadOnly)) {
        info.exists = false;
        return info;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        info.exists = false;
        return info;
    }

    QJsonObject root = doc.object();
    info.dayNumber = root["dayNumber"].toInt();
    info.timestamp = root["timestamp"].toString();

    QJsonObject playerObj = root["player"].toObject();
    info.money = playerObj["money"].toInt();

    return info;
}

bool SaveManager::saveGame(int saveSlot, Player* player, TileBoard* tiles, int dayNumber) {
    if (!player || !tiles) {
        std::cerr << "SaveManager: Cannot save - null player or tiles" << std::endl;
        return false;
    }

    ensureSaveDirectoryExists();

    QJsonObject root;
    root["version"] = "1.0";
    root["dayNumber"] = dayNumber;
    root["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    root["player"] = serializePlayer(player);
    root["tiles"] = serializeTiles(tiles);

    QJsonDocument doc(root);

    QString path = getSavePath(saveSlot);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        std::cerr << "SaveManager: Failed to open file for writing: " << path.toStdString() << std::endl;
        return false;
    }

    file.write(doc.toJson());
    file.close();

    std::cout << "SaveManager: Game saved to slot " << saveSlot << std::endl;
    return true;
}

bool SaveManager::loadGame(int saveSlot, Player* player, TileBoard* tiles, int& dayNumber) {
    if (!player || !tiles) {
        std::cerr << "SaveManager: Cannot load - null player or tiles" << std::endl;
        return false;
    }

    QString path = getSavePath(saveSlot);
    if (!QFile::exists(path)) {
        std::cerr << "SaveManager: Save file does not exist: " << path.toStdString() << std::endl;
        return false;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "SaveManager: Failed to open file for reading: " << path.toStdString() << std::endl;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        std::cerr << "SaveManager: Failed to parse JSON" << std::endl;
        return false;
    }

    QJsonObject root = doc.object();
    dayNumber = root["dayNumber"].toInt();

    if (!deserializePlayer(root["player"].toObject(), player)) {
        std::cerr << "SaveManager: Failed to deserialize player" << std::endl;
        return false;
    }

    if (!deserializeTiles(root["tiles"].toObject(), tiles)) {
        std::cerr << "SaveManager: Failed to deserialize tiles" << std::endl;
        return false;
    }

    std::cout << "SaveManager: Game loaded from slot " << saveSlot << std::endl;
    return true;
}

QJsonObject SaveManager::serializePlayer(Player* player) const {
    QJsonObject obj;
    obj["x"] = player->getX();
    obj["y"] = player->getY();
    obj["money"] = player->getMoney();
    obj["state"] = player->getState();
    obj["selectedSlot"] = player->getSelectedSlot();
    obj["inventory"] = serializeInventory(player->getInventory());
    return obj;
}

QJsonObject SaveManager::serializeInventory(Inventory* inventory) const {
    QJsonObject obj;
    QJsonArray slotsArray;

    const auto& invSlots = inventory->getSlots();
    for (const auto& slot : invSlots) {
        QJsonObject slotObj;
        slotObj["itemType"] = static_cast<int>(slot.itemType);
        slotObj["quantity"] = slot.quantity;
        slotsArray.append(slotObj);
    }

    obj["slots"] = slotsArray;
    return obj;
}

QJsonObject SaveManager::serializeTiles(TileBoard* tiles) const {
    QJsonObject obj;
    obj["width"] = tiles->width();
    obj["height"] = tiles->height();

    QJsonArray tilesArray;
    for (int y = 0; y < tiles->height(); ++y) {
        QJsonArray row;
        for (int x = 0; x < tiles->width(); ++x) {
            ITiles* tile = tiles->getTile(x, y);
            if (tile) {
                QJsonObject tileObj;
                tileObj["type"] = QString::fromStdString(tile->getType());
                tileObj["state"] = tile->getState();
                tileObj["watered"] = tile->getWatered();

                if (tile->hasCrop() && tile->getCrop()) {
                    tileObj["crop"] = serializeCrop(tile->getCrop());
                }

                if (tile->hasAnimal() && tile->getAnimal()) {
                    tileObj["animal"] = serializeAnimal(tile->getAnimal());
                }

                row.append(tileObj);
            } else {
                row.append(QJsonValue());
            }
        }
        tilesArray.append(row);
    }

    obj["tiles"] = tilesArray;
    return obj;
}

QJsonObject SaveManager::serializeCrop(ICrops* crop) const {
    QJsonObject obj;
    obj["type"] = QString::fromStdString(crop->getType());
    obj["state"] = crop->getState();
    obj["growthSpeed"] = crop->getGrowthSpeed();
    obj["harvestable"] = crop->getHarvestable();
    return obj;
}

QJsonObject SaveManager::serializeAnimal(IAnimal* animal) const {
    QJsonObject obj;
    obj["type"] = QString::fromStdString(animal->getType());
    obj["state"] = animal->getState();
    obj["foodType"] = QString::fromStdString(animal->getFoodType());
    obj["harvestable"] = animal->getHarvestable();
    return obj;
}

bool SaveManager::deserializePlayer(const QJsonObject& json, Player* player) {
    player->setX(json["x"].toDouble());
    player->setY(json["y"].toDouble());
    player->setMoney(json["money"].toInt());
    player->setState(json["state"].toInt());
    player->setSelectedSlot(json["selectedSlot"].toInt());

    return deserializeInventory(json["inventory"].toObject(), player);
}

bool SaveManager::deserializeInventory(const QJsonObject& json, Player* player) {
    QJsonArray slotsArray = json["slots"].toArray();
    Inventory* inventory = player->getInventory();

    for (int i = 0; i < Inventory::INVENTORY_SIZE; ++i) {
        ItemTypeEnum currentItem = inventory->getItemInSlot(i);
        int currentQty = inventory->getItemCountInSlot(i);
        if (currentItem != ItemTypeEnum::NONE && currentQty > 0) {
            inventory->removeItemFromSlot(i, currentQty);
        }
    }

    for (int i = 0; i < slotsArray.size() && i < Inventory::INVENTORY_SIZE; ++i) {
        QJsonObject slotObj = slotsArray[i].toObject();
        ItemTypeEnum itemType = static_cast<ItemTypeEnum>(slotObj["itemType"].toInt());
        int quantity = slotObj["quantity"].toInt();

        if (itemType != ItemTypeEnum::NONE && quantity > 0) {
            inventory->addItem(itemType, quantity);
        }
    }

    return true;
}

bool SaveManager::deserializeTiles(const QJsonObject& json, TileBoard* tiles) {
    QJsonArray tilesArray = json["tiles"].toArray();

    for (int y = 0; y < tilesArray.size() && y < tiles->height(); ++y) {
        QJsonArray row = tilesArray[y].toArray();
        for (int x = 0; x < row.size() && x < tiles->width(); ++x) {
            QJsonObject tileObj = row[x].toObject();
            if (tileObj.isEmpty()) continue;

            ITiles* tile = tiles->getTile(x, y);
            if (!tile) continue;

            QString tileType = tileObj["type"].toString();
            int state = tileObj["state"].toInt();
            bool watered = tileObj["watered"].toBool();

            if (tile->getType() != tileType.toStdString()) {
                ITiles* newTile = nullptr;
                if (tileType == "Field") {
                    newTile = new Field();
                } else if (tileType == "Yard") {
                    newTile = new Yard();
                }

                if (newTile) {
                    tiles->setTile(x, y, newTile);
                    tile = newTile;
                }
            }

            tile->setState(state);
            tile->setWatered(watered);

            if (tileObj.contains("crop")) {
                ICrops* crop = deserializeCrop(tileObj["crop"].toObject());
                if (crop) {
                    tile->setCrop(crop);
                }
            }

            if (tileObj.contains("animal")) {
                IAnimal* animal = deserializeAnimal(tileObj["animal"].toObject());
                if (animal) {
                    tile->setAnimal(animal);
                }
            }
        }
    }

    return true;
}

ICrops* SaveManager::deserializeCrop(const QJsonObject& json) {
    QString cropType = json["type"].toString();
    ICrops* crop = CropFactory::createCrop(cropType.toStdString());

    if (crop) {
        int state = json["state"].toInt();
        int growthSpeed = json["growthSpeed"].toInt();
        bool harvestable = json["harvestable"].toBool();

        for (int i = 0; i < state; ++i) {
            crop->setState();
        }
        crop->setGrowthSpeed(growthSpeed);
        crop->setHarvestable(harvestable);
    }

    return crop;
}

IAnimal* SaveManager::deserializeAnimal(const QJsonObject& json) {
    QString animalType = json["type"].toString();
    IAnimal* animal = AnimalFactory::createAnimal(animalType.toStdString());

    if (animal) {
        int state = json["state"].toInt();
        animal->setState(state);
        animal->setHarvestable();
    }

    return animal;
}
