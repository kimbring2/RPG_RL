#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Entity.h"
#include "Math.h"

class Hole : public Entity
{
    public: 
    Hole(Vector2f p_pos, SDL_Texture* p_tex);
    virtual void Test() {std::cout << "Hole Test Function...\n";};
    private:
};

class Hole_Test : public Entity
{
    public: 
    Hole_Test(int x, int y);
    virtual void Test() {std::cout << "Hole Test Function...\n";};
    private:
};