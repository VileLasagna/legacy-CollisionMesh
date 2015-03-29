#ifndef SDL_BITMAP_H
#define SDL_BITMAP_H

#include <SDL_image.h>
#include <string>

class SDLBitmap
{
public:
	SDLBitmap();

	bool Load(const std::string& filename);

    unsigned int GetColourAtPoint(int x, int y) const;

protected:
	SDL_Surface* m_image;
};

#endif
