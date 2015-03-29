#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H


#include "Vec3.h"

#include <utility>
#ifdef _WIN32
#include <windows.h>
#endif //WIN32
#include <GL/gl.h>

class LineSeg
{

public:
	LineSeg() {p0 = Vec3f(0,0,0); p1 = Vec3f(0,0,0);}

	LineSeg(const Vec3f& a, const Vec3f& b) {p0 = a; p1 = b;}
	LineSeg(float ax, float ay, float az, float bx, float by, float bz){ p0 = Vec3f(ax,ay,az); p1 = Vec3f(bx,by,bz);}


	Vec3f getStart() const {return p0;}
	Vec3f getEnd() const {return p1;}
	Vec3f GetStart() const {return p0;}
	Vec3f GetEnd() const {return p1;}

	void Draw() ;

	bool contains(const Vec3f& point) const {}
	std::pair<bool, Vec3f> intersects (const LineSeg& other) const ;
	bool parallel(const LineSeg& other) const;
	float sqDistToPoint ( const Vec3f& p) const;


private:

	Vec3f p0;
	Vec3f p1;




};
#endif //LINE_SEG_H