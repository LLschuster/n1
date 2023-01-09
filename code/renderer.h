#ifndef RENDERER_H
#define RENDERER_H

namespace ng
{
    struct DrawDungeonSprites
    {
        Sprite *floorSprite;
        Sprite *wallSprite;
    };

    class Renderer : public EngineSystem
    {
    public:
        SDL_Window *window;
        SDL_Renderer *renderer;

        static Renderer *getInstance()
        {
            // this var gets created and initialize only on the first getInstance call
            static Renderer *instance = nullptr;
            if (!instance)
            {
                instance = new Renderer(); // todo get memory from memorypool
            }
            return instance;
        }
        void shutdown()
        {
            TTF_Quit();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
        uint32 startup();

        void drawRect(float x, float y, float w, float h);

        void drawSprite(Sprite *sprite);

        void drawDungeon(DrawDungeonSprites sprites, DungeonRoom *rootRoom);

    private:
        Renderer() {}
        ~Renderer() {}
    };
};

#endif
