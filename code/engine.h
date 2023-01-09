#ifndef ENGINE_H
#define ENGINE_H

#include "ng_math.h"

namespace ng
{
    static const char *DATA_PATH = "..//data//";

    struct Size
    {
        int h;
        int w;
    };

    struct Position
    {
        int32 x;
        int32 y;
    };

    struct Sprite
    {
        Position position;
        int32 height;
        int32 width;
        SDL_Rect *textCoord;
        SDL_Texture *texture;
    };

    struct Player 
    {
        int32 speed;
        Position cameraOffset;
        Sprite *sprite;
    };

    struct DungeonCorridor{
        Position startPos;
        Position endPos;
    };

    struct DungeonRoom
    {
        int32 id;
        int32 height;
        int32 width;
        int32 centerX;
        int32 centerY;
        DungeonCorridor corridor;
        DungeonRoom *subRoom;
        DungeonRoom *sisterRoom;
    };

    class EngineSystem
    {
        std::string type;
        virtual uint32 startup() { return 0; };
        virtual void shutdown(){};
    };

    DungeonRoom *createDungeon(uint32 height, uint32 width, uint32 minRoomHeight, uint32 minRoomWidth);
}

#endif
