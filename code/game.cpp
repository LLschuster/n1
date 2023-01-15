

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <main.h>
#include <math.h>
#include <algorithm>
#include <stdint.h>
#include <time.h>
#include <stdarg.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <cstring>
#include <map>
#include <vector>
#include <cmath>

typedef uint32_t uint32;
typedef uint8_t uint8;
typedef int32_t int32;

#include "engine.h"
#include "main.h"
#include "ng_math.h"
#include "renderer.h"

#define lg ng::Logger::getInstance()
#define rd ng::Renderer::getInstance()
#define rm ng::ResourceManager::getInstance()
#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 1920

struct GameState
{
    bool gameRunning = true;
    ng::Player player;
    ng::DungeonManager dungeonManager;
} gameState;

#include "profile.cpp"
#include "ng_math.cpp"
#include "utils.cpp"
#include "engine.cpp"
#include "sprite.cpp"
#include "renderer.cpp"
#include "resourceManager.cpp"

void handleGameEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    { // poll until all events are handled!
        // decide what to do with this event.
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                gameState.gameRunning = false;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_W)
            {
                gameState.player.cameraOffset.y -= gameState.player.speed;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_S)
            {
                gameState.player.cameraOffset.y += gameState.player.speed;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_A)
            {
                gameState.player.cameraOffset.x -= gameState.player.speed;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_D)
            {
                gameState.player.cameraOffset.x += gameState.player.speed;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_C)
            {
                gameState.player.cameraOffset.x += gameState.dungeonManager.rootRoom->centerX + gameState.player.sprite->position.x;
            }
        }
    }
}

int main(int argc, char **argv)
{
    int initResult = 0;
    initResult += lg->startup();
    initResult += rd->startup();
    initResult += rm->startup();

    if (initResult > 0)
    {
        lg->writeLog("Systems initialization failed");
        exit(initResult);
    }

    rm->loadTextures();

    {
        ng::Player player;
        ng::Sprite dragonSprite = ng::createSprite(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 100, 100, rm->getTexture("dragon.png"));
        player.sprite = &dragonSprite;
        player.speed = 100;
        player.cameraOffset = {dragonSprite.position.x, dragonSprite.position.y};
        gameState.player = player;
    }

    gameState.dungeonManager.init();
    struct SDL_Rect floorTileTextCoord = {2 * 32, 6 * 32, 32, 32};
    ng::Sprite floorTileSprite = ng::createSprite(120, 140, 100, 100, rm->getTexture("dg_dungeon32.gif"), &floorTileTextCoord);
    struct SDL_Rect wallTileTextCoord = {1 * 32, 0 * 32, 32, 32};
    ng::Sprite wallTileSprite = ng::createSprite(120, 140, 100, 100, rm->getTexture("dg_dungeon32.gif"), &wallTileTextCoord);
    ng::DrawDungeonSprites dungeonSprites = {&floorTileSprite, &wallTileSprite};

    gameState.dungeonManager.rootRoom = ng::createDungeon(4000, 4000, dungeonSprites.floorSprite->height * 8, dungeonSprites.floorSprite->width * 8);

    while (gameState.gameRunning)
    {
        handleGameEvents();
        SDL_SetRenderDrawColor(rd->renderer, 255, 180, 255, 255);
        SDL_RenderClear(rd->renderer);

        rd->drawDungeon(dungeonSprites, gameState.dungeonManager.rootRoom);
        rd->drawSprite(gameState.player.sprite);
        SDL_RenderPresent(rd->renderer);

        SDL_Delay(16);
    }

    gameState.dungeonManager.destroyDungeon();
    lg->shutdown();
    rd->shutdown();
    rm->shutdown();

    return 0;
}
