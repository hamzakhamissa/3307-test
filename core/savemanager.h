#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QStandardPaths>

// Forward declarations
class Player;
class TileBoard;
class Inventory;
class ITiles;
class ICrops;
class IAnimal;

class SaveManager {
public:
    // Singleton access
    static SaveManager& getInstance();

    SaveManager(const SaveManager&) = delete;
    SaveManager& operator=(const SaveManager&) = delete;

    bool saveGame(int saveSlot, Player* player, TileBoard* tiles, int dayNumber);
    bool loadGame(int saveSlot, Player* player, TileBoard* tiles, int& dayNumber);

    bool saveExists(int saveSlot) const;
    QString getSavePath(int saveSlot) const;
    bool deleteSave(int saveSlot);

    struct SaveInfo {
        int slot;
        int dayNumber;
        int money;
        QString timestamp;
        bool exists;
    };
    SaveInfo getSaveInfo(int saveSlot) const;

private:
    SaveManager(); 
    ~SaveManager();

    QJsonObject serializePlayer(Player* player) const;
    QJsonObject serializeTiles(TileBoard* tiles) const;
    QJsonObject serializeInventory(Inventory* inventory) const;
    QJsonObject serializeCrop(ICrops* crop) const;
    QJsonObject serializeAnimal(IAnimal* animal) const;

    bool deserializePlayer(const QJsonObject& json, Player* player);
    bool deserializeTiles(const QJsonObject& json, TileBoard* tiles);
    bool deserializeInventory(const QJsonObject& json, Player* player);
    ICrops* deserializeCrop(const QJsonObject& json);
    IAnimal* deserializeAnimal(const QJsonObject& json);

    QString getAppDataPath() const;
    void ensureSaveDirectoryExists() const;
};

#endif 
