#ifndef TILEBOARD_H
#define TILEBOARD_H

#include <vector>
#include <memory>
#include <QPainter>
#include <QPixmap>
#include "itiles.h"
#include "field.h"
#include "yard.h"
#include "water.h"
#include "character.h"

// Forward declarations
class Player;
class IInteract;

class TileBoard {
public:
    TileBoard(int width, int height, int tileSize);
    ~TileBoard();

    bool init(const QString& grassTexPath,
              const QString& dirtTexPath,
              const QString& seedTexPath,
              const QString& plantPhase1TexPath,
              const QString& plantPhase2TexPath,
              const QString& plantPhase3TexPath);

    void render(QPainter& painter, float cameraX, float cameraY, int viewWidth, int viewHeight);
    void interactAt(int x, int y, Player* player);
    void advanceGrowth();

    bool inBounds(int x, int y) const;
    int width() const { return boardWidth; }
    int height() const { return boardHeight; }

    ITiles* getTile(int x, int y);
    void setTile(int x, int y, ITiles* tile);

private:
    int boardWidth;
    int boardHeight;
    int tileSize;

    std::vector<std::vector<std::unique_ptr<ITiles>>> tiles;

    // Textures
    QPixmap grassTex;
    QPixmap dirtTex;
    QPixmap seedTex;
    QPixmap plantPhase1Tex;
    QPixmap plantPhase2Tex;
    QPixmap plantPhase3Tex;
    QPixmap wheatStage1Tex;
    QPixmap wheatStage2Tex;
    QPixmap wheatStage3Tex;

    QPixmap tomatoStage1Tex;
    QPixmap tomatoStage2Tex;
    QPixmap tomatoStage3Tex;

    QPixmap cornStage1Tex;
    QPixmap cornStage2Tex;
    QPixmap cornStage3Tex;
};

#endif // TILEBOARD_H

