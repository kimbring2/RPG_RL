#include "Ball.h"
#include "Entity.h"
#include "Math.h"
#include "Tile.h"
#include "Hole.h"

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <cstdlib>

#include "viz_shared_memory.h"


std::string ball_action;
bool swing_flag = false;
bool animation_flag = false;
bool start_flag_ball = true;
bool next_level_flag = false;


Ball::Ball(Vector2f p_pos, SDL_Texture* p_tex, SDL_Texture* p_pointTex, SDL_Texture* p_powerMTexFG, SDL_Texture* p_powerMTexBG, int p_index)
:Entity(p_pos, p_tex)
{
    index = p_index;
    points.push_back(Entity(Vector2f(-64, -64), p_pointTex));
    powerBar.push_back(Entity(Vector2f(-64, -64), p_powerMTexBG));
    powerBar.push_back(Entity(Vector2f(-64, -64), p_powerMTexFG));
}


void Ball::setVelocity(float x, float y)
{
    velocity.x = x;
    velocity.y = y;
}


void Ball::setLaunchedVelocity(float x, float y)
{
    launchedVelocity.x = x;
    launchedVelocity.y = y;
}


void Ball::setInitialMousePos(float x, float y)
{
    initialMousePos.x = x;
    initialMousePos.y = y;
}


void Ball::setWin(bool p_win)
{
    win = p_win;

}


void Ball::update(double deltaTime, bool mouseDown, bool mousePressed, std::vector<Tile> tiles,std::vector<Hole> holes, Mix_Chunk* chargeSfx, 
                  Mix_Chunk* swingSfx, Mix_Chunk* holeSfx, unsigned int ball_index)
{   
    deltaTime = 2.0;

    VIZ_GetInputState();

    std::string command = ball_action;

    if ( ( animation_flag == true) and (ball_index == 0) ) {
        float velocity_x = getVelocity().x;
        float velocity_y = getVelocity().y;

        if ( (velocity_x == 0.0) and (velocity_y == 0.0) ){
            animation_flag = false;
            swing_flag = false;
        }
    }


    if ( ( command[0] == '1' ) and (swing_flag == false) and (ball_index == 0) and (start_flag_ball == true) )
    {
        swing_flag = true;

        char velocity_x_buf[5];
        for (int i = 0; i < 5; i++ ) {
           velocity_x_buf[i] = command[3 + i];
        }

        char velocity_y_buf[5];
        for (int i = 0; i < 5; i++ ) {
           velocity_y_buf[i] = command[11 + i];
        }

        float velocity_x = std::atof(velocity_x_buf);
        if (command[2] == '-') {
            velocity_x = -velocity_x;
        }

        float velocity_y = std::atof(velocity_y_buf);
        if (command[10] == '-') {
            velocity_y = -velocity_y;
        }

        setVelocity(velocity_x, velocity_y);
        setLaunchedVelocity(velocity_x, velocity_y);
        velocity1D = SDL_sqrt(SDL_pow(abs(getVelocity().x), 2) + SDL_pow(abs(getVelocity().y), 2));
        launchedVelocity1D = velocity1D;

        points.at(0).setPos(getPos().x, getPos().y + 8 - 32);
        points.at(0).setAngle(SDL_atan2(velocity.y, velocity.x) * (180 / 3.1415) + 90);

        dirX = velocity.x / abs(velocity.x);
        dirY = velocity.y / abs(velocity.y);
        if (velocity1D > 1)
        {
            velocity1D = 1;
            launchedVelocity1D = 1;
        }

        canMove = true;

        strokes++;
        Mix_PlayChannel(-1, swingSfx, 0);
    } 
    else if ( command[0] == '2' )
    {
        //canMove = false;
        ;
    }

    if (win)
    {
        VIZ_GameStateUpdateReward(1.0);

        if (getPos().x < target.x)
        {
            setPos(getPos().x += 0.1 * deltaTime, getPos().y);
        }
        else if (getPos().x > target.x)
        {
            setPos(getPos().x -= 0.1 * deltaTime, getPos().y);
        }

        if (getPos().y < target.y)
        {
            setPos(getPos().x, getPos().y += 0.1 * deltaTime);
        }
        else if (getPos().y > target.y)
        {
            setPos(getPos().x, getPos().y -= 0.1 * deltaTime);
        }

        setScale(getScale().x - 0.001 * deltaTime, getScale().y - 0.001 * deltaTime);

        next_level_flag = true;

        return;
    }
    
    for (Hole h : holes)
    {
        if (getPos().x + 4 > h.getPos().x && getPos().x + 16 < h.getPos().x + 20 && getPos().y + 4 > h.getPos().y && getPos().y + 16 < h.getPos().y + 20)
        {
            Mix_PlayChannel(-1, holeSfx, 0);
            setWin(true);
            target.x = h.getPos().x;
            target.y = h.getPos().y + 3;
        }
    }

    if (mousePressed && canMove)
    {
        Mix_PlayChannel(-1, chargeSfx, 0);
        playedSwingFx = false;
        int mouseX = 0;
	    int mouseY = 0;
	    SDL_GetMouseState(&mouseX, &mouseY);
        setInitialMousePos(mouseX, mouseY);
    }

    if (mouseDown && canMove)
    {
        int mouseX = 0;
	    int mouseY = 0;
	    SDL_GetMouseState(&mouseX, &mouseY);

        float velocity_x = (mouseX - getInitialMousePos().x) / -150;
        float velocity_y = (mouseY - getInitialMousePos().y) / -150;

        setVelocity((mouseX - getInitialMousePos().x) / -150, (mouseY - getInitialMousePos().y) / -150);
        setLaunchedVelocity((mouseX - getInitialMousePos().x) / -150, (mouseY - getInitialMousePos().y) / -150);
        velocity1D = SDL_sqrt(SDL_pow(abs(getVelocity().x), 2) + SDL_pow(abs(getVelocity().y), 2));
        launchedVelocity1D = velocity1D;
 
        points.at(0).setPos(getPos().x, getPos().y + 8 - 32);
        points.at(0).setAngle(SDL_atan2(velocity.y, velocity.x) * (180 / 3.1415) + 90);

        dirX = velocity.x / abs(velocity.x);
        dirY = velocity.y / abs(velocity.y);

        powerBar.at(0).setPos(getPos().x + 32 + 8, getPos().y - 32);
        powerBar.at(1).setPos(getPos().x + 32 + 8 + 4, getPos().y - 32 + 4 + 32 - 32 * powerBar.at(1).getScale().y);
        if (velocity1D > 1)
        {
            velocity1D = 1;
            launchedVelocity1D = 1;
        }

        powerBar.at(1).setScale(1, velocity1D / 1);
    }
    else
    {
        if (!playedSwingFx)
        {
            Mix_PlayChannel(-1, swingSfx, 0);
            playedSwingFx = true;
            strokes++;
        }

        points.at(0).setPos(-64, -64);
        powerBar.at(0).setPos(-64, -64);
        powerBar.at(1).setPos(-64, -64);
        canMove = false;
        setPos(getPos().x + getVelocity().x * deltaTime, getPos().y + getVelocity().y * deltaTime);
        if (getVelocity().x > 0.0001 || getVelocity().x < -0.0001 || getVelocity().y > 0.0001 || getVelocity().y < -0.0001)
        {
            if (velocity1D > 0)
            {
                float deltaTimeTest = 2.0;
                velocity1D -= friction * deltaTime;
            }
            else
            {
                velocity1D = 0;
            }

            velocity.x = (velocity1D / launchedVelocity1D) * abs(launchedVelocity.x) * dirX;
            velocity.y = (velocity1D / launchedVelocity1D) * abs(launchedVelocity.y) * dirY;
        }
        else
        {
            setVelocity(0,0);
            int mouseX = 0;
	        int mouseY = 0;
            SDL_GetMouseState(&mouseX, &mouseY);
            setInitialMousePos(mouseX, mouseY);
            canMove = true;
        }

        if (getPos().x + getCurrentFrame().w > 640 / (2 - index))
        {
            setVelocity(-abs(getVelocity().x), getVelocity().y);
            dirX = -1;
        }
        else if (getPos().x < 0 + (index * 320))
        {
            setVelocity(abs(getVelocity().x), getVelocity().y);
            dirX = 1;
        }
        else if (getPos().y + getCurrentFrame().h > 480)
        {
            setVelocity(getVelocity().x, -abs(getVelocity().y));
            dirY = -1;
        }
        else if (getPos().y < 0)
        {
            setVelocity(getVelocity().x, abs(getVelocity().y));
            dirY = 1;
        }

        for (Tile& t : tiles)
	    { 
		    float newX = getPos().x + getVelocity().x * deltaTime;
            float newY = getPos().y;
            if (newX + 16 > t.getPos().x && newX < t.getPos().x + t.getCurrentFrame().w && newY + 16 > t.getPos().y && newY < t.getPos().y + t.getCurrentFrame().h - 3)
            {
                setVelocity(getVelocity().x * -1, getVelocity().y);
                dirX *= -1;
            }

            newX = getPos().x;
            newY = getPos().y + getVelocity().y * deltaTime;
            if (newX + 16 > t.getPos().x && newX < t.getPos().x + t.getCurrentFrame().w && newY + 16 > t.getPos().y && newY < t.getPos().y + t.getCurrentFrame().h - 3)
            {
                setVelocity(getVelocity().x, getVelocity().y * -1);
                dirY *= -1;
            }
	    }
    }
}