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

    struct DungeonManager 
    {
        DungeonRoom *rootRoom;
        std::vector<DungeonRoom*> leafRooms;
        void init(){
            leafRooms.reserve(1000);
            rootRoom = nullptr;
        }
        void freeRoom(DungeonRoom *room){
            room->subRoom = NULL;
            room->sisterRoom = NULL;
            free(room);
        }
        void destroyDungeon(){
            leafRooms.empty();
            std::vector<DungeonRoom*> toFree;
            toFree.reserve(1000);
            toFree.push_back(rootRoom);
            while (toFree.size() > 0){
                auto currentRoom = toFree[toFree.size() - 1];
                toFree.pop_back();

                if (!currentRoom){
                    continue;
                }

                if (currentRoom->subRoom){
                    toFree.push_back(currentRoom->subRoom);
                    toFree.push_back(currentRoom->subRoom->sisterRoom);
                }

                freeRoom(currentRoom);
            }
            rootRoom = nullptr;
        }
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
