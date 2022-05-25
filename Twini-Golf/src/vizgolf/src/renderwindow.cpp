#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "RenderWindow.h"
#include "Entity.h"
#include "viz_shared_memory.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;


RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
	:window(NULL), renderer(NULL)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}


SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)
{
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);

	if (texture == NULL)
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;

	return texture;
}


void RenderWindow::cleanUp()
{
	SDL_DestroyWindow(window);
}


void RenderWindow::clear()
{
	SDL_RenderClear(renderer);
}


void RenderWindow::render(Entity& p_entity)
{
	SDL_Rect src; 
	src.x = p_entity.getCurrentFrame().x;
	src.y = p_entity.getCurrentFrame().y;
	src.w = p_entity.getCurrentFrame().w;
	src.h = p_entity.getCurrentFrame().h;

	SDL_Rect dst;
	dst.x = p_entity.getPos().x + (p_entity.getCurrentFrame().w - p_entity.getCurrentFrame().w * p_entity.getScale().x) / 2;
	dst.y = p_entity.getPos().y + (p_entity.getCurrentFrame().h - p_entity.getCurrentFrame().h * p_entity.getScale().y) / 2;
	dst.w = p_entity.getCurrentFrame().w * p_entity.getScale().x;
	dst.h = p_entity.getCurrentFrame().h * p_entity.getScale().y;

	SDL_RenderCopyEx(renderer, p_entity.getTex(), &src, &dst, p_entity.getAngle(), 0, SDL_FLIP_NONE);
}


void RenderWindow::render(int x, int y, SDL_Texture* p_tex)
{
	SDL_Rect src; 
	src.x = 0;
	src.y = 0;
	src.w;
	src.h;

	SDL_QueryTexture(p_tex, NULL, NULL, &src.w, &src.h);

	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = src.w;
	dst.h = src.h;

	SDL_RenderCopy(renderer, p_tex, &src, &dst);
}


void RenderWindow::render(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
{
	SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, textColor);
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = surfaceMessage->w;
	src.h = surfaceMessage->h; 

	SDL_Rect dst;
	dst.x = p_x;
	dst.y = p_y;
	dst.w = src.w;
	dst.h = src.h;

	SDL_RenderCopy(renderer, message, &src, &dst);
	SDL_FreeSurface(surfaceMessage);
 	SDL_DestroyTexture(message);
}


void RenderWindow::renderCenter(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
{
	//std::string p_text_;
	//p_text_ = std::string(p_text);
	//std::cout << "p_text_: " << p_text_ << "\n";
	//printf("p_text: %s\n", p_text);

	SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, textColor);
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = surfaceMessage->w;
	src.h = surfaceMessage->h; 

	SDL_Rect dst;
	dst.x = 640 / 2 - src.w / 2 + p_x;
	dst.y = 480 / 2 - src.h / 2 + p_y;
	dst.w = src.w;
	dst.h = src.h;

	SDL_RenderCopy(renderer, message, &src, &dst);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
}


unsigned char *matToBytes(Mat image)
{
   int size = image.total() * image.elemSize();
   //printf("size: %d\n", size);

   unsigned char *bytes = new unsigned char[size];  // you will have to delete[] that later
   std::memcpy(bytes, image.data,size * sizeof(unsigned char));

   return bytes;
}


void RenderWindow::display()
{
	SDL_RenderPresent(renderer);

	SDL_Surface *formattedImage = SDL_GetWindowSurface(window);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, formattedImage->pixels, formattedImage->pitch);

    //VideoWriter outputVideo;
	Uint32* textureBuffer = new Uint32[formattedImage->pitch * formattedImage->h];

    Uint32 height = formattedImage->w;
    Uint32 width = formattedImage->h;
    Uint32 pitch = formattedImage->pitch;

    /* Draw a gradient on the surf and append a new frame in the video each new line */
    SDL_LockSurface(formattedImage);

    Uint32 *pixels = (Uint32*)formattedImage->pixels;
	for(int y = 0; y < height; y++)
	    for(int x = 0; x < width; x++)
	        textureBuffer[width * y + x] = pixels[width * y + x];

    Mat img(width, height, CV_8UC4, textureBuffer);

	unsigned char *buf = matToBytes(img);

    VIZ_ScreenBufferUpdate(buf);

    SDL_FreeSurface(formattedImage);
}