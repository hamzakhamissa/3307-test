#ifndef MENU_H
#define MENU_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>

// Forward declarations
class Player;
class TileBoard;
class Inventory;
class ITiles;
class ICrops;
class Game;

class Menu {
public:
    Menu();
    ~Menu();

    // Save game state
    bool save(const QString& filePath, Player* player, TileBoard* tiles, int dayNumber);
    
    // Load game state
    bool load(const QString& filePath, Player* player, TileBoard* tiles, int& dayNumber);
    
    // Get default save path
    static QString getDefaultSavePath(int saveSlot = 1);
    
    // Check if save file exists
    static bool saveExists(int saveSlot);

private:
    QString filePath;
    
    // Helper methods for serialization
    QJsonObject serializePlayer(Player* player);
    QJsonObject serializeTiles(TileBoard* tiles);
    QJsonObject serializeInventory(Inventory* inventory);
    
    // Helper methods for deserialization
    bool deserializePlayer(const QJsonObject& json, Player* player);
    bool deserializeTiles(const QJsonObject& json, TileBoard* tiles);
    bool deserializeInventory(const QJsonObject& json, Player* player);
};

#endif // MENU_H

