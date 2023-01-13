namespace ng
{
    uint32 Renderer::startup()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        {
            printf("SDL could not be initialized %s", SDL_GetError());
            exit(2);
        }

        SDL_Window *sdlWindow = SDL_CreateWindow("TBD",
                                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                                 SCREEN_HEIGHT, 0);

        if (sdlWindow == NULL)
        {
            printf("SDL window could not be initialized %s", SDL_GetError());
            exit(2);
        }

        SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow,
                                                       -1, SDL_RENDERER_PRESENTVSYNC);

        if (sdlRenderer == NULL)
        {
            printf("SDL window could not be initialized %s", SDL_GetError());
            exit(2);
        }

        window = sdlWindow;
        renderer = sdlRenderer;
        return 0;
    }

    void Renderer::drawRect(float x, float y, float w, float h)
    {
        SDL_Rect rect = {};
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        SDL_RenderDrawRect(renderer, &rect);
    }

    void Renderer::drawSprite(Sprite *sprite)
    {
        SDL_Rect destRect;
        destRect.x = sprite->position.x;
        destRect.y = sprite->position.y;
        destRect.h = sprite->height;
        destRect.w = sprite->width;

        SDL_RenderCopy(renderer, sprite->texture, sprite->textCoord, &destRect);
    }

    void Renderer::drawDungeon(DrawDungeonSprites sprites, DungeonRoom *rootRoom)
    {
        Timer("drawDungeon");
        std::vector<DungeonRoom *> toCheckRooms;
        toCheckRooms.push_back(rootRoom);
        while (toCheckRooms.size() > 0)
        {
            auto currentRoom = toCheckRooms[toCheckRooms.size() - 1];
            toCheckRooms.pop_back();

            if (currentRoom == nullptr)
            {
                continue;
            }

            if (!currentRoom->subRoom)
            {
                {
                    int32 tilesWidthCount = (currentRoom->width / sprites.floorSprite->width) - 4; // TODO sprite has constant width??
                    int32 tilesHeightCount = (currentRoom->height / sprites.floorSprite->height) - 4;
                    // int32 restWidthTile = currentRoom->width % (uint32)sprite->width;
                    // int32 restHeightTile = currentRoom->height % (uint32)sprite->height;

                    for (int32 x = 0; x < tilesWidthCount; x++)
                    {
                        for (int32 y = 0; y < tilesHeightCount; y++)
                        {
                            Sprite *toDrawSprite;
                            if (x == 0 || y == 0 || x == tilesWidthCount - 1 || y == tilesHeightCount - 1)
                            {
                                toDrawSprite = sprites.wallSprite;
                            }
                            else
                            {
                                toDrawSprite = sprites.floorSprite;
                            }
                            toDrawSprite->position.x = currentRoom->centerX - ((x - tilesWidthCount / 2) * toDrawSprite->width) - gameState.player.cameraOffset.x;
                            toDrawSprite->position.y = currentRoom->centerY - ((y - tilesHeightCount / 2) * toDrawSprite->height) - gameState.player.cameraOffset.y;
                            drawSprite(toDrawSprite);
                        }
                    }
                }

                {
                    if (currentRoom->corridor.startPos.x == currentRoom->corridor.endPos.x){
                        // Draw vertical corridor
                        int32 tileCount = std::abs(currentRoom->corridor.startPos.y - currentRoom->corridor.endPos.y) / sprites.floorSprite->height;
                        int32 direction = currentRoom->corridor.startPos.y - currentRoom->corridor.endPos.y > 0 ? -1 : 1;
                        for (int32 x = 0; x <= tileCount; x++){
                            Sprite *toDrawSprite = sprites.floorSprite;
                            toDrawSprite->position.x = currentRoom->corridor.startPos.x - gameState.player.cameraOffset.x;
                            toDrawSprite->position.y = currentRoom->corridor.startPos.y + (direction * (x * toDrawSprite->height)) - gameState.player.cameraOffset.y;
                            drawSprite(toDrawSprite);
                        }
                    } else {
                        // Draw horizontal corridor
                        int32 tileCount = std::abs(currentRoom->corridor.startPos.x - currentRoom->corridor.endPos.x) / sprites.floorSprite->width;
                        int32 direction = currentRoom->corridor.startPos.x - currentRoom->corridor.endPos.x > 0 ? -1 : 1;
                        for (int32 x = 0; x <= tileCount; x++){
                            Sprite *toDrawSprite = sprites.floorSprite;
                            toDrawSprite->position.x = currentRoom->corridor.startPos.x + (direction * (x * toDrawSprite->width)) - gameState.player.cameraOffset.x;
                            toDrawSprite->position.y = currentRoom->corridor.startPos.y - gameState.player.cameraOffset.y;
                            drawSprite(toDrawSprite);
                        }
                    }
                }
            }
            else
            {
                toCheckRooms.push_back(currentRoom->subRoom->sisterRoom);
                toCheckRooms.push_back(currentRoom->subRoom);
            }
        }
    }
}
