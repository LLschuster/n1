

namespace ng
{


    Sprite
    createSprite(int32 x, int32 y, int32 _height, int32 _width, SDL_Texture *_texture, SDL_Rect *_textCoord)
    {
        Timer timer("createSprite");
        Sprite newSprite;
        Position position;
        position.x = x;
        position.y = y;
        newSprite.position = position;

        newSprite.height = _height;
        newSprite.width = _width;
        newSprite.texture = _texture;
        newSprite.textCoord = _textCoord;

        return newSprite;
    }

}
