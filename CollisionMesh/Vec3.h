//#ifndef VEC_3_H
//#define VEC_3_H
//
//template <class T, class RESULT = float>
//struct Vec3
//{
//	T x, y, z;
//	Vec3(T px = 0, T py = 0, T pz = 0) : x(px), y(py), z(pz) {}
//
//	T X() const {return x;}
//	T Y() const {return y;}
//	T Z() const {return z;}
//
//	RESULT Length() const
//	{
//		return sqrt(SquaredLength());
//	}
//
//	RESULT SquaredLength() const
//	{
//		return x * x + y * y + z * z;
//	}
//
//    Vec3<T, RESULT>& operator+=(const Vec3<T, RESULT>& v)
//    {
//        x += v.x;
//        y += v.y;
//        z += v.z;
//        return *this;
//    }
//
//    Vec3<T, RESULT>& operator*=(T f)
//    {
//        x *= f;
//        y *= f;
//        z *= f;
//        return *this;
//    }
//
//	RESULT DotProduct(const Vec3<T, RESULT>& v)
//	{
//		return x * v.x    + y * v.y    + z * v.z;
//	}
//
//	Vec3<T, RESULT> CrossProduct(const Vec3<T, RESULT>& v)
//	{
//		return Vec3<T, RESULT>(
//			y * v.z - z * v.y,
//			z * v.x - x * v.z,
//			x * v.y - y * v.x);
//	}
//
//
//    void Normalize()
//    {
//        RESULT r = Length();
//        x /= r;
//        y /= r;
//        z /= r;
//    }
//};
//
//template <class T>
//Vec3<T> operator+(const Vec3<T>& v1, const Vec3<T>& v2)
//{
//    return Vec3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
//}
//
//template <class T>
//Vec3<T> operator*(const Vec3<T>& v, T t)
//{
//    return Vec3<T>(v.x * t, v.y * t, v.z * t);
//}
//
//template <class T, class RESULT>
//Vec3<T, RESULT> operator-(const Vec3<T, RESULT>& u, const Vec3<T, RESULT>& v)
//{
//	return Vec3<T, RESULT>(u.X() - v.X(), u.Y() - v.Y(), u.Z() - v.Z());
//}
//
//
//typedef Vec3<float, float> Vec3f;
//
//#endif



#ifndef VEC_3_H
#define VEC_3_H

#include <math.h>

template <class T, class RESULT = float>
struct Vec3
{
	T x, y, z;
	Vec3(T px = 0, T py = 0, T pz = 0) : x(px), y(py), z(pz) {}

	RESULT Length() const
	{
		return sqrt(SquaredLength());
	}

	RESULT SquaredLength() const
	{
		return x * x + y * y + z * z;
	}

	RESULT sqMod()const
	{
		return SquaredLength();
	}

	T X() const {return x;}
	T Y() const {return y;}
	T Z() const {return z;}

    Vec3<T, RESULT>& operator+=(const Vec3<T, RESULT>& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

	Vec3<T, RESULT>& operator-=(const Vec3<T, RESULT>& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }


    Vec3<T, RESULT>& operator*=(T f)
    {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

	RESULT DotProduct(const Vec3<T, RESULT>& v) const
	{
		return x * v.x    + y * v.y    + z * v.z;
	}

	Vec3<T, RESULT> CrossProduct(const Vec3<T, RESULT>& v)
	{
		return Vec3<T, RESULT>(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x);
	}

	Vec3<T, RESULT> operator-(const Vec3<T, RESULT>& v) const
	{
		return Vec3<T, RESULT>(x - v.x, y - v.y, z - v.z);
	}

	Vec3<T, RESULT> operator/(const T& t)
	{
		x /= t;
		y /= t;
		x /= t;
		return *this;
	}

    void Normalize()
    {
        RESULT r = Length();
        x /= r;
        y /= r;
        z /= r;
    }

	void normalise()
	{
		Normalize();
	}
};

template <class T, class RESULT>
Vec3<T, RESULT> operator+(const Vec3<T, RESULT>& v1, const Vec3<T, RESULT>& v2)
{
    return Vec3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

template <class T, class RESULT>
Vec3<T, RESULT> operator*(const Vec3<T, RESULT>& v, T t)
{
    return Vec3<T>(v.x * t, v.y * t, v.z * t);
}

template <class T, class RESULT>
RESULT dotProd(const Vec3<T,RESULT>& a, const Vec3<T,RESULT>& b)
{
	return a.DotProduct(b);
}

template <class T, class RESULT>
bool operator== (const Vec3<T,RESULT>& a, const Vec3<T,RESULT>& b)
{
    return ( (a.X() == b.X())&&(a.Y() == b.Y()) && (a.Z() == b.Z()));
}


typedef Vec3<float, float> Vec3f;

#endif
