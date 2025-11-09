#include "menu.h"
#include "player.h"
#include "tileboard.h"
#include "inventory.h"
#include "itiles.h"
#include "icrops.h"
#include <QDir>
#include <QStandardPaths>
#include <iostream>

Menu::Menu() {}

Menu::~Menu() {}

QString Menu::getDefaultSavePath(int saveSlot) {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(appDataPath)) {
        dir.mkpath(appDataPath);
    }
    return appDataPath + QString("/save_%1.json").arg(saveSlot);
}

bool Menu::saveExists(int saveSlot) {
    QString path = getDefaultSavePath(saveSlot);
    return QFile::exists(path);
}

bool Menu::save(const QString& filePath, Player* player, TileBoard* tiles, int dayNumber) {
    if (!player || !tiles) return false;

    QJsonObject gameState;
    gameState["dayNumber"] = dayNumber;
    gameState["player"] = serializePlayer(player);
    gameState["tiles"] = serializeTiles(tiles);

    QJsonDocument doc(gameState);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        std::cerr << "Failed to open save file for writing: " << filePath.toStdString() << std::endl;
        return false;
    }

    file.write(doc.toJson());
    file.close();
    return true;
}

bool Menu::load(const QString& filePath, Player* player, TileBoard* tiles, int& dayNumber) {
    if (!player || !tiles) return false;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Failed to open save file for reading: " << filePath.toStdString() << std::endl;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        std::cerr << "Failed to parse save file JSON" << std::endl;
        return false;
    }

    QJsonObject gameState = doc.object();
    dayNumber = gameState["dayNumber"].toInt();

    if (!deserializePlayer(gameState["player"].toObject(), player)) {
        std::cerr << "Failed to deserialize player" << std::endl;
        return false;
    }

    if (!deserializeTiles(gameState["tiles"].toObject(), tiles)) {
        std::cerr << "Failed to deserialize tiles" << std::endl;
        return false;
    }

    return true;
}

QJsonObject Menu::serializePlayer(Player* player) {
    QJsonObject obj;
    obj["x"] = player->getX();
    obj["y"] = player->getY();
    obj["money"] = player->getMoney();
    obj["state"] = player->getState();
    obj["selectedSlot"] = player->getSelectedSlot();
    obj["inventory"] = serializeInventory(player->getInventory());
    return obj;
}

QJsonObject Menu::serializeTiles(TileBoard* tiles) {
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
                if (tile->hasCrop()) {
                    ICrops* crop = tile->getCrop();
                    if (crop) {
                        QJsonObject cropObj;
                        cropObj["type"] = QString::fromStdString(crop->getType());
                        cropObj["state"] = crop->getState();
                        cropObj["harvestable"] = crop->getHarvestable();
                        tileObj["crop"] = cropObj;
                    }
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

QJsonObject Menu::serializeInventory(Inventory* inventory) {
    QJsonObject obj;
    QJsonArray slotsArray;

    // FIXED: Renamed 'slots' to 'invSlots' to avoid Qt macro conflict
    const auto& invSlots = inventory->getSlots();
    for (size_t i = 0; i < invSlots.size(); ++i) {
        QJsonObject slotObj;
        slotObj["itemType"] = static_cast<int>(invSlots[i].itemType);
        slotObj["quantity"] = invSlots[i].quantity;
        slotsArray.append(slotObj);
    }

    obj["slots"] = slotsArray;
    return obj;
}

bool Menu::deserializePlayer(const QJsonObject& json, Player* player) {
    player->setX(json["x"].toDouble());
    player->setY(json["y"].toDouble());
    player->setMoney(json["money"].toInt());
    player->setState(json["state"].toInt());
    player->setSelectedSlot(json["selectedSlot"].toInt());

    return deserializeInventory(json["inventory"].toObject(), player);
}

bool Menu::deserializeTiles(const QJsonObject& json, TileBoard* tiles) {
    // Note: This is a simplified version. Full implementation would need to recreate tiles
    // For now, we'll just restore basic state
    QJsonArray tilesArray = json["tiles"].toArray();

    for (int y = 0; y < tilesArray.size() && y < tiles->height(); ++y) {
        QJsonArray row = tilesArray[y].toArray();
        for (int x = 0; x < row.size() && x < tiles->width(); ++x) {
            QJsonObject tileObj = row[x].toObject();
            ITiles* tile = tiles->getTile(x, y);
            if (tile) {
                tile->setState(tileObj["state"].toInt());
                tile->setWatered(tileObj["watered"].toBool());

                // Restore crop if present
                if (tileObj.contains("crop")) {
                    QJsonObject cropObj = tileObj["crop"].toObject();
                    QString cropType = cropObj["type"].toString();
                    // Create appropriate crop type (simplified - would need full crop creation)
                    // This is a placeholder - full implementation needed
                }
            }
        }
    }

    return true;
}

bool Menu::deserializeInventory(const QJsonObject& json, Player* player) {
    QJsonArray slotsArray = json["slots"].toArray();
    Inventory* inventory = player->getInventory();

    // Clear and rebuild inventory
    // Note: This is simplified - full implementation would properly restore inventory
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
