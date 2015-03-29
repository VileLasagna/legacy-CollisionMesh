#include "SDLBitmap.h"
#include <assert.h>
#include <iostream>

SDLBitmap::SDLBitmap()
{
	m_image = 0;
}

bool SDLBitmap::Load(const std::string &filename)
{
	m_image = IMG_Load(filename.c_str());
    std::cout << SDL_GetError() << "\n";

	return (m_image != 0);
}

unsigned int SDLBitmap::GetColourAtPoint(int x, int y) const
{
    assert(m_image);
    assert(x >= 0);
    assert(y >= 0);
    assert(x < m_image->w);
    assert(y < m_image->h);

    unsigned char* p = (unsigned char*)(m_image->pixels) + 
        (x + y * m_image->w) * m_image->format->BytesPerPixel;

    unsigned int r = p[0] + (p[1] << 8) + (p[2] << 16);

    return r;
}

