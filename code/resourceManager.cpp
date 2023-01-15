
namespace ng
{
    void ResourceManager::loadTextures()
    {
        Timer timer("Load game textures");
        char graphicsPath[200];
        memset(graphicsPath, 0, sizeof(graphicsPath));
        concatStrings(graphicsPath, (char*)DATA_PATH, "g//");

        for (char *resourceName : graphicsResources)
        {
            char buffer[200];
            memset(buffer, 0, sizeof(buffer));
            concatStrings(buffer, graphicsPath, resourceName);
            SDL_Surface *surface = IMG_Load(buffer);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(rd->renderer, surface);
            resourceMap[resourceName] = texture;

            SDL_FreeSurface(surface);
        }
    }

    SDL_Texture *ResourceManager::getTexture(char *key){
        for (auto pair : resourceMap){
            if (strcmp(key, pair.first) == 0){
                return pair.second;
            }
        }
    }
}
