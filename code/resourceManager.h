#ifndef RESOURCE_MANAGER
#define RESOURCE_MANAGER

namespace ng
{
    std::vector<char*> graphicsResources = {
        "bread.png",
        "coin.png",
        "crystal.png",
        "dragon.png",
        "goblin1.png",
        "grass.png",
        "mage1.png",
        "mage2.png",
        "minerals.png",
        "skull.png",
        "soldier.png",
        "stone.png",
        "water.png",
        "wolf.png",
        "wood.png",
        "dg_dungeon32.gif",
    };

    class ResourceManager : public EngineSystem
    {
    public:
        std::map<char*, SDL_Texture*> resourceMap;

        static ResourceManager *getInstance()
        {
            static ResourceManager *instance = nullptr;
            if (!instance)
            {
                instance = new ResourceManager();
            }
            return instance;
        }

        ResourceManager() {}

        uint32 startup()
        {
            Timer("ResourceManager::startup");
            for (char *resourceName : graphicsResources)
            {
                resourceMap[resourceName] = nullptr;
            }
            return 0;
        }

        void shutdown()
        {
            for (auto it = resourceMap.begin(); it != resourceMap.end(); it++)
            {
                if (it->second != nullptr)
                {
                    SDL_DestroyTexture(it->second);
                }
            }
            resourceMap.clear();
        }

        void loadTextures();
        SDL_Texture *getTexture(char *key);
    };
}

#endif