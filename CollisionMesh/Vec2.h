#ifndef VEC2_H
#define VEC2_H

template<class T>
struct Vec2
{
	Vec2(T x = 0, T y = 0) : m_x(x), m_y(y)
	{
	}

	Vec2& operator+=(const Vec2& v2)
	{
		m_x += v2.m_x;
		m_y += v2.m_y;
		return *this;
	}

	T m_x;
	T m_y;
};

// Overloaded operators for Vec2
// Multiply each component of the Vec2 by m
template<class T>
Vec2<T> operator*(const Vec2<T>& v, T m)
{
	return Vec2<T>(v.m_x * m, v.m_y * m);
}

typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;

#endif
