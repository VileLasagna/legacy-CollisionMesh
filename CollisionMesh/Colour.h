#ifndef COLOUR_H
#define COLOUR_H

#include <windows.h>
#include <GL/gl.h>

struct Colour
{
	Colour(float pr = 1.0f, float pg = 1.0f, float pb = 1.0f, float pa = 1.0f) : 
		r(pr), g(pg), b(pb), a(pa)
		{
		}

	void Draw()
	{
		glColor4f(r, g, b, a);
	}

	float r, g, b, a;
};

#endif
