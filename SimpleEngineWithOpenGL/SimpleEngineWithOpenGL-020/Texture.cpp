#include "Texture.h"
#include "Log.h"
#include <SDL_image.h>
#include <sstream>

Texture::Texture(): textureID(0), filename(""), width(0), height(0), SDLTexture(nullptr)
{
}

Texture::~Texture()
{
	// TODO solve
	if (!SDLTexture)
	{
		SDL_DestroyTexture(SDLTexture);
	}
	//glDeleteTextures(1, &textureID);
}

bool Texture::loadSDL(RendererSDL& renderer, const string& filenameP)
{
	filename = filenameP;
	// Load from file
	SDL_Surface* surf = IMG_Load(filename.c_str());
	if (!surf)
	{
		Log::error(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture file " + filename);
		return false;
	}
	width = surf->w;
	height = surf->h;

	// Create texture from surface
	SDLTexture = SDL_CreateTextureFromSurface(renderer.toSDLRenderer(), surf);
	SDL_FreeSurface(surf);
	if (!SDLTexture)
	{
		Log::error(SDL_LOG_CATEGORY_RENDER, "Failed to convert surface to texture for "+ filename);
		return false;
	}
	Log::info("Loaded texture " + filename);
	return true;
}

bool Texture::loadOGL(RendererOGL& renderer, const string& filenameP)
{
	filename = filenameP;
	// Load from file
	SDL_Surface* surf = IMG_Load(filename.c_str());
	if (!surf)
	{
		Log::error(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture file " + filename);
		return false;
	}
	width = surf->w;
	height = surf->h;
	int format = GL_RGBA;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surf->pixels);
	SDL_FreeSurface(surf);


	Log::info("Loaded texture " + filename);
	// Enable bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::updateInfo(int& widthOut, int& heightOut)
{
	widthOut = width;
	heightOut = height;
}

void Texture::setActive() const
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}